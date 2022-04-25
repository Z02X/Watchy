#include "WatchyExpanded.h"

// Expanded
#include "WatchyApp.h"

// ESP
#include <esp_sleep.h>

// Wire
#include <Wire.h>

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

// WiFi
#include <WiFi.h>

RTC_DATA_ATTR SExpandedData g_data;

CWatchyExpanded::CWatchyExpanded() : m_display(GxEPD2_154_D67(wcd::kCS, wcd::kDC, wcd::kReset, wcd::kBusy)),
                                     m_data(g_data) {}

void CWatchyExpanded::Run()
{
	const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
	Wire.begin(SDA, SCL); //init i2c

	// Init the display here for all cases, if unused, it will do nothing
	m_display.init(0, m_data.m_init, 10, true); // 10ms by spec, and fast pulldown reset
	//Init();
	m_display.epd2.setBusyCallback(DisplayBusyCallback);

	bool bPartialRefresh = true;
	switch (wakeup_reason)
	{
		case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
			if(m_data.m_guiState == SExpandedData::guiState::face)
				m_UpdateWatchFace = true;
		break;
		case ESP_SLEEP_WAKEUP_EXT1: //button Press
			HandleButtonPress();
		break;
		default: //reset
			m_UpdateWatchFace = true;
			bPartialRefresh = false;
		break;
	}

	if (m_UpdateWatchFace)
		UpdateScreen(bPartialRefresh);
	DeepSleep();
}

CWatchyExpanded::ADisplay& CWatchyExpanded::Display()
{
	return m_display;
}

CExpandedRTC& CWatchyExpanded::RTC()
{
	return m_rtc;
}

tm& CWatchyExpanded::Time()
{
	m_rtc.Read(m_Time);
	return m_Time;
}

bool CWatchyExpanded::ConnectWiFi() // TODO: Move to service
{
	//WiFi not setup, you can also use hard coded credentials with WiFi.begin(SSID,PASS);
	if (wl_status_t::WL_CONNECT_FAILED == WiFi.begin())
		return false;

	WiFi.waitForConnectResult();

	return true;
}

void CWatchyExpanded::Init()
{
	if (!m_data.m_init)
		return;

	m_data.m_init = false;
}

void CWatchyExpanded::DisplayBusyCallback(const void*)
{
	gpio_wakeup_enable(static_cast<gpio_num_t>(wcd::kBusy), GPIO_INTR_LOW_LEVEL);
	esp_sleep_enable_gpio_wakeup();
	esp_light_sleep_start();
}

void CWatchyExpanded::UpdateScreen(const bool partial_update)
{
	m_display.setFullWindow();
	m_faces[m_data.m_face % m_faces.size()]->Get()->Draw(*this);
	m_display.display(partial_update); //partial refresh
	m_data.m_guiState = SExpandedData::guiState::face;
}

void CWatchyExpanded::DeepSleep()
{
	m_display.hibernate();
	m_data.m_init = false;

	m_rtc.NextMinuteWake();

	for(int i = 0; i < 40; ++i) // Set pins 0-39 to input to avoid power leaking out, may reset interface to RTC
		pinMode(i, INPUT);

	esp_sleep_enable_ext0_wakeup(wcp::rtc_pin, 0); //enable deep sleep wake on RTC interrupt
	esp_sleep_enable_ext1_wakeup(wcp::btn_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
	esp_deep_sleep_start();
}

void CWatchyExpanded::HandleButtonPress()
{
	const uint64_t kWakeupBit = esp_sleep_get_ext1_wakeup_status();
	if (m_data.m_guiState == SExpandedData::guiState::face)
	{
		if (kWakeupBit & wcp::up_btn_mask)
			BackFace();
		else if (kWakeupBit & wcp::down_btn_mask)
			ForwardFace();
		else if (kWakeupBit & wcp::menu_btn_mask)
			Menu();
	}
	else if (m_data.m_guiState == SExpandedData::guiState::menu)
	{
		if (kWakeupBit & wcp::back_btn_mask)
		{
			m_data.m_guiState = SExpandedData::guiState::face;
			m_UpdateWatchFace = true;
		}
		else if (kWakeupBit & wcp::menu_btn_mask)
		{
			m_apps[m_data.m_menuItem % m_apps.size()]->Get()->Work(*this);
		}
		else if (kWakeupBit & wcp::up_btn_mask)
		{
			--m_data.m_menuItem;
		}
		else if (kWakeupBit & wcp::down_btn_mask)
		{
			++m_data.m_menuItem;
		}

		Menu();
	}
}

void CWatchyExpanded::Menu()
{
	m_display.setFullWindow();
	m_display.fillScreen(GxEPD_BLACK);
	m_display.setFont(&FreeMonoBold9pt7b);
	m_display.setCursor(0, 16);

	const std::size_t Size = m_apps.size();
	for (std::size_t i = 0; i < Size; ++i)
	{
		CHolderBase<CWatchyApp>* pApp = m_apps[i];
		const char* const Name = pApp->Get()->Name();
		m_display.setCursor(m_display.getCursorX() + 1, m_display.getCursorY());
		if (m_data.m_menuItem % Size == i)
		{
			int16_t boundaryX, boundaryY;
			uint16_t w, h;
			m_display.getTextBounds(Name, m_display.getCursorX(), m_display.getCursorY(), &boundaryX, &boundaryY, &w, &h);
			m_display.setTextColor(GxEPD_BLACK);
			m_display.fillRect(boundaryX-1, boundaryY-1, w+2, h+2, GxEPD_WHITE);
		}
		else
		{
			m_display.setTextColor(GxEPD_WHITE);
		}

		m_display.println(Name);
	}
	m_data.m_guiState = SExpandedData::guiState::menu;
	m_display.display(true);
}

void CWatchyExpanded::BackFace()
{
	--m_data.m_face;
	m_UpdateWatchFace = true;
}

void CWatchyExpanded::ForwardFace()
{
	++m_data.m_face;
	m_UpdateWatchFace = true;
}

void CWatchyExpanded::_bmaConfig()
{
	//if (m_sensor.begin(_readRegister, _writeRegister, delay) == false)
	//	return; //fail to init BMA
}

uint16_t CWatchyExpanded::_readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom((uint8_t)address, (uint8_t)len);
	uint8_t i = 0;
	while (Wire.available())
		data[++i] = Wire.read();
	return 0;
}

float CWatchyExpanded::BatteryVoltage() // TODO Check for bug on watchy main
{
	//if(m_rtc.rtcType == DS3231)
	//	return analogReadMilliVolts(wcp::batt_adc_pin) / 1000.0f * 2.0f; // Battery voltage goes through a 1/2 divider.
	return analogReadMilliVolts(wcp::batt_adc_pin) / 1000.0f * 2.0f;
}
