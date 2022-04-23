#pragma once

// RTCs
#include <DS3232RTC.h>
#include <Rtc_Pcf8563.h>

// STL
#include <ctime>

class CExpandedRTC
{
	public:
		CExpandedRTC();
		void ClearAlarm();
		void NextMinuteWake();
		void Read(std::tm& time);
		void Set(tmElements_t& tm);

	private:
		enum ERTCType : std::uint8_t
		{
			Unknown,
			DS3231,
			PCF8563
		} m_rtcType = ERTCType::Unknown;

		ERTCType GetRTCType();

		DS3232RTC rtc_ds;
		Rtc_Pcf8563 rtc_pcf;
};
