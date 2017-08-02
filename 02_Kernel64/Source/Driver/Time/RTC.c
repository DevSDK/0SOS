#include "RTC.h"

void ReadRTCTime(BYTE* _Out_Hour, BYTE* _Out_Minute, BYTE* _Out_Second)
{
    BYTE data;
    //CMOS 메모리에서 시간에 관련된 정보를 읽어오는 함수 값은 BCD 코드
    PortIO_OutByte(PORT_CMOS_ADDRESS, CMOS_ADDRESS_RTC_HOUR);
    data = PortIO_InByte(PORT_CMOS_DATA);
    *_Out_Hour = BCD_TO_BIN(data);

    PortIO_OutByte(PORT_CMOS_ADDRESS, CMOS_ADDRESS_RTC_MINUTE);
    data = PortIO_InByte(PORT_CMOS_DATA);
    *_Out_Minute = BCD_TO_BIN(data);

    PortIO_OutByte(PORT_CMOS_ADDRESS, CMOS_ADDRESS_RTC_SECOND);
    data = PortIO_InByte(PORT_CMOS_DATA);
    *_Out_Second = BCD_TO_BIN(data);    
}
void ReadRTCDate(WORD* _Out_Year, BYTE* _Out_Month, BYTE* _Out_DayOfMonth, BYTE* _Out_DayOfWeek)
{
    BYTE data;
    //CMOS 메모리에서 날자와 관련된 정보를 읽어오는 함수 값은 BCD 코드
    PortIO_OutByte(PORT_CMOS_ADDRESS, CMOS_ADDRESS_RTC_YEAR);
    data = PortIO_InByte(PORT_CMOS_DATA);
    *_Out_Year = BCD_TO_BIN(data) + 2000; //연도에 2000을 더해 2000년도를 표현

    PortIO_OutByte(PORT_CMOS_ADDRESS, CMOS_ADDRESS_RTC_MONTH);
    data = PortIO_InByte(PORT_CMOS_DATA);
    *_Out_Month = BCD_TO_BIN(data);

    PortIO_OutByte(PORT_CMOS_ADDRESS, CMOS_ADDRESS_RTC_DAY_OF_MONTH);
    data = PortIO_InByte(PORT_CMOS_DATA);
    *_Out_DayOfMonth = BCD_TO_BIN(data);
    
    PortIO_OutByte(PORT_CMOS_ADDRESS, CMOS_ADDRESS_RTC_DAY_OF_WEEK);
    data = PortIO_InByte(PORT_CMOS_DATA);
    *_Out_DayOfWeek = BCD_TO_BIN(data);    
}
char* ConvertDayOfWeekString(BYTE _DayOfWeek)
{
    //날자를 요일 문자열로 바꿔주는 함수
    static char* WeekStringTable [8] = {"NULL", "Sunday", "Monday", "Tushday", "Wednesday", "Thursday", "Friday", "Saturday"};

    if(_DayOfWeek >= 8)
        return WeekStringTable[0];

    return WeekStringTable[_DayOfWeek];

}
