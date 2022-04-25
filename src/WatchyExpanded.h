#pragma once

// STL
#include <vector>
#include <ctime>

// GxEPD2
#include <GxEPD2_BW.h>

// Time
#include <TimeLib.h>

// Expanded
#include "watchy_config.h"
#include "WatchFace.h"
#include "ExpandedRTC.h"
#include "Holder.h"

// Defs
class CWatchFace;
class CWatchyApp;

struct SExpandedData
{
	bool m_init = true;
	std::uint8_t m_face = 0;

	enum guiState : std::uint8_t
	{
		face,
		menu
	} m_guiState = guiState::face;

	std::uint8_t m_menuItem = 0;
};

class CWatchyExpanded
{
	public:
		using ADisplay = GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>;

		CWatchyExpanded();

		template <typename T>
		void AddWatchFace()
		{
			m_faces.push_back(new CHolder<T, CWatchFace>);
		}

		template <typename T>
		void AddApp()
		{
			m_apps.push_back(new CHolder<T, CWatchyApp>);
		}

		void Init();
		void Run();

		bool ConnectWiFi();

		ADisplay& Display();
		CExpandedRTC& RTC();
		tm& Time();
		float BatteryVoltage();

	private:
		static void DisplayBusyCallback(const void*);

		void UpdateScreen(const bool fullUpdate);
		void DeepSleep();

		void HandleButtonPress();

		void BackFace();
		void ForwardFace();

		void Menu();

		void _bmaConfig();
		static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);

		std::vector<CHolderBase<CWatchFace>*> m_faces;
		std::vector<CHolderBase<CWatchyApp>*> m_apps;

		ADisplay m_display;
		std::tm  m_Time; // Follow the cpp standard first and others after.

		CExpandedRTC m_rtc;
		SExpandedData& m_data;
		bool m_UpdateWatchFace = false;
		//BMA423 m_sensor;
};
