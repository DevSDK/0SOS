#ifndef __RTC_H__
#define __RTC_H__
#include <Types.h>

#define  PORT_CMOS_ADDRESS      0x70
#define  PORT_CMOS_DATA         0x71

#define  CMOS_ADDRESS_RTC_SECOND       0x00
#define  CMOS_ADDRESS_RTC_MINUTE        0x02
#define  CMOS_ADDRESS_RTC_HOUR          0x04
#define  CMOS_ADDRESS_RTC_DAY_OF_WEEK   0x06
#define  CMOS_ADDRESS_RTC_DAY_OF_MONTH  0x07
#define  CMOS_ADDRESS_RTC_MONTH         0x08
#define  CMOS_ADDRESS_RTC_YEAR          0x09

//BCD 코드 변환 - BCD 코드: 1바이트를 4비트씩 잘라서 10진수 2자리로 표현 ex) 0010 0001 이면 21
#define  BCD_TO_BIN(x) ((((x) >> 4) * 10) + ((x)  & 0x0F ))
void ReadRTCTime(BYTE* _Out_Hour, BYTE* _Out_Minute, BYTE* _Out_Second);
void ReadRTCDate(WORD* _Out_Year, BYTE* _Out_Month, BYTE* _Out_DayOfMonth, BYTE* _Out_DayOfWeek);
char* ConvertDayOfWeekString(BYTE _DayOfWeek);

#endif /*__RTC_H__*/