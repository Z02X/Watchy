// Self
#include "ExpandedRTC.h"

constexpr uint16_t rtcDS_addr{0x68};
constexpr uint16_t rtcPCF_addr{0x51};

CExpandedRTC::CExpandedRTC() {}

void CExpandedRTC::ClearAlarm()
{
	if(GetRTCType() == DS3231)
	{
		rtc_ds.alarm(DS3232RTC::ALARM_2);
	}
	else if (GetRTCType() == PCF8563)
	{
		rtc_pcf.clearAlarm(); //resets the alarm flag in the RTC
		int nextAlarmMinute = rtc_pcf.getMinute();
		nextAlarmMinute = (nextAlarmMinute == 59) ? 0 : (nextAlarmMinute + 1); //set alarm to trigger 1 minute from now
		rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);
	}
}

void CExpandedRTC::NextMinuteWake()
{
	if (GetRTCType() == ERTCType::DS3231)
	{
		rtc_ds.setAlarm(DS3232RTC::ALARM_TYPES_t::ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
		rtc_ds.clearAlarm(DS3232RTC::ALARM_NBR_t::ALARM_2); //resets the alarm flag in the RTC
	}
	else if (GetRTCType() == ERTCType::PCF8563)
	{
		rtc_pcf.clearAlarm(); //resets the alarm flag in the RTC
		rtc_pcf.setAlarm(constrain((rtc_pcf.getMinute() + 1), 0, 59), 99, 99, 99);
	}
}

void CExpandedRTC::Read(tmElements_t &tm)
{
	if(GetRTCType() == ERTCType::DS3231)
	{
		rtc_ds.read(tm);
	}
	else if (GetRTCType() ==PCF8563)
	{
		tm.Year = y2kYearToTm(rtc_pcf.getYear());
		tm.Month = rtc_pcf.getMonth();
		tm.Day = rtc_pcf.getDay();
		//TimeLib & DS3231 has Wday range of 1-7, but PCF8563 stores day of week in 0-6 range
		tm.Wday = rtc_pcf.getWeekday() + 1;
		tm.Hour = rtc_pcf.getHour();
		tm.Minute = rtc_pcf.getMinute();
		tm.Second = rtc_pcf.getSecond();
	}
}

void CExpandedRTC::Set(tmElements_t& tm)
{
	if(GetRTCType() == DS3231)
	{
		time_t t = makeTime(tm);
		rtc_ds.set(t);
	}
	else if (GetRTCType() == PCF8563)
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

uint8_t CExpandedRTC::CExpandedRTC::GetRTCType()
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
		m_rtcType =ERTCType::DS3231;
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
