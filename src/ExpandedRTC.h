#pragma once

#include <DS3232RTC.h>
#include <Rtc_Pcf8563.h>

class CExpandedRTC
{
	public:
		CExpandedRTC();
		void ClearAlarm();
		void NextMinuteWake();
		void Read(tmElements_t &tm);
		void Set(tmElements_t& tm);

	private:
		uint8_t GetRTCType();

		enum ERTCType : uint8_t
		{
			Unknown,
			DS3231,
			PCF8563
		} m_rtcType = ERTCType::Unknown;

		DS3232RTC rtc_ds;
		Rtc_Pcf8563 rtc_pcf;
};
