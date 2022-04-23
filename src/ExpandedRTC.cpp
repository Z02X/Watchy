// Self
#include "ExpandedRTC.h"

// Time
#include <TimeLib.h>

constexpr uint16_t rtcDS_addr{0x68};
constexpr uint16_t rtcPCF_addr{0x51};

CExpandedRTC::CExpandedRTC() {}

void CExpandedRTC::ClearAlarm()
{
	const ERTCType rctType = GetRTCType();
	if(rctType == ERTCType::DS3231)
	{
		rtc_ds.alarm(DS3232RTC::ALARM_2);
	}
	else if (rctType == ERTCType::PCF8563)
	{
		rtc_pcf.clearAlarm(); //resets the alarm flag in the RTC
	}
}

void CExpandedRTC::NextMinuteWake()
{
	const ERTCType rctType = GetRTCType();
	if (rctType == ERTCType::DS3231)
	{
		rtc_ds.setAlarm(DS3232RTC::ALARM_TYPES_t::ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
		rtc_ds.clearAlarm(DS3232RTC::ALARM_NBR_t::ALARM_2); //resets the alarm flag in the RTC
	}
	else if (rctType == ERTCType::PCF8563)
	{
		const byte NextMin = (rtc_pcf.getMinute() + 1) % 60;
		rtc_pcf.clearAlarm(); //resets the alarm flag in the RTC
		rtc_pcf.setAlarm(NextMin, 99, 99, 99);
	}
}

void CExpandedRTC::Read(std::tm &time)
{
	const ERTCType rctType = GetRTCType();
	if(rctType == ERTCType::DS3231) // Needs testing
	{
		// DS3231 has Wday range of 1-7
		tmElements_t tm;
		rtc_ds.read(tm);
		const std::time_t epocTime = makeTime(tm);
		time = *localtime(&epocTime);
		//time = *gmtime(&epocTime);
	}
	else if (rctType == ERTCType::PCF8563)
	{
		time.tm_sec = rtc_pcf.getSecond();
		time.tm_min = rtc_pcf.getMinute();
		time.tm_hour = rtc_pcf.getHour();
		time.tm_mday = rtc_pcf.getDay();
		time.tm_mon = rtc_pcf.getMonth() - 1; // PCF8563 stores month 1-12
		time.tm_year = 100 + rtc_pcf.getYear(); // PCF8563 year starts on 2000
		time.tm_wday = rtc_pcf.getWeekday(); // PCF8563 stores day of week in 0-6 range
	}
	time.tm_isdst = -1; // Boo DST, but in all seriousness this needs to be resolved
}

void CExpandedRTC::Set(tmElements_t& tm)
{
	const ERTCType rctType = GetRTCType();
	if(rctType == ERTCType::DS3231)
	{
		time_t t = makeTime(tm);
		rtc_ds.set(t);
	}
	else if (rctType == ERTCType::PCF8563)
	{
		time_t t = makeTime(tm); //make and break to calculate tm.Wday
		breakTime(t, tm);
		//day, weekday, month, century(1=1900, 0=2000), year(0-99)
		//TimeLib & DS3231 has Wday range of 1-7, but PCF8563 stores day of week in 0-6 range
		rtc_pcf.setDate(tm.Day, tm.Wday - 1, tm.Month, 0, tmYearToY2k(tm.Year));

		//hr, min, sec
		rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);
	}

	ClearAlarm();
}

CExpandedRTC::ERTCType CExpandedRTC::CExpandedRTC::GetRTCType()
{
	if (m_rtcType != ERTCType::Unknown)
		return m_rtcType;

	Wire.beginTransmission(rtcDS_addr);
	if(Wire.endTransmission() == 0)
	{
		uint8_t controlReg = rtc_ds.readRTC(0x0E);
		const uint8_t mask = _BV(7);
		if (controlReg & mask)
		{
			controlReg &= ~mask;
			rtc_ds.writeRTC(0x0E, controlReg);
		}
		m_rtcType = ERTCType::DS3231;
	}
	else
	{
		Wire.beginTransmission(rtcPCF_addr);
		if(Wire.endTransmission() == 0)
		{
			rtc_pcf.clearStatus();
			m_rtcType = ERTCType::PCF8563;
		}
	}

	return m_rtcType;
}
