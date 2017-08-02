#include "Shell.h"
#include <Driver/Keyboard/Keyboard.h>
#include <Driver/VGA/IO_VGA.h>
#include <Utility/String.h>
#include <Console/Console.h>
/*
    셸의 엔트리포인트
    __KernelEntry 에서 초기화가 끝나면 진입하는 함수
*/
void StartShell()
{
    char CommandBuffer[SHELL_INPUT_BUFFER_SIZE];
    _SetCursor(0, 17);
    int CommandBufferIndex = 0;
    _Printf(SHELL_PROMPT_MESSAGE);
    const int Prompt_length = sizeof(SHELL_PROMPT_MESSAGE);
	while(1)
	{
       	BYTE c = _GetCh();
        //백스페이스 처리
        if(c == KEY_BACKSPACE)
        {
            if(CommandBufferIndex > 0)
            {
                int x, y;
                _GetCursor(&x, &y);
                int dx = x - Prompt_length;   
                //커서의 위치에따른 백스페이스를 처리하기 위해 지운 위치에서
                //커멘드 버퍼를 비우고 다시 출력함
                for(int i = dx; i < CommandBufferIndex; i++)
                {
                    CommandBuffer[i] = CommandBuffer[i + 1];
                }
                for(int i = 0; i< CONSOLE_WIDTH; i++)
                {
                    _PrintStringXY(i,y, __GetConsole_System().current_attribute, " ");    
                }
                

                _SetCursor(0, y);
                _Printf(SHELL_PROMPT_MESSAGE);
                _Printf("%s", CommandBuffer);
                if((x-1) >= Prompt_length)
                    _SetCursor(x-1, y);
                else
                    _SetCursor(x, y);
               
                CommandBufferIndex --;
                //마지막 문자일시
                if(CommandBufferIndex == 0)
                {
                    _SetCursor(Prompt_length-1, y);                    
                }

            }
        }
        else if(c == KEY_ENTER)
        {
            _Printf("\n");
            //커멘드 버퍼에 커멘드가 있다면, 커멘드를 넘김
            if(CommandBufferIndex > 0)
            {
                CommandBuffer[CommandBufferIndex] = '\0';
                ExecuteCommand(CommandBuffer);
            }
            _Printf(SHELL_PROMPT_MESSAGE);
            CommandBufferIndex = 0;
            _MemSet(CommandBuffer,0 , SHELL_INPUT_BUFFER_SIZE);
        }
        else if( (c == KEY_LSHIFT) || (c == KEY_RSHIFT) || (c == KEY_CAPSLOCK)
                || (c == KEY_NUMLOCK) || (c == KEY_SCROLLLOCK))
        {
            ;
        }
        //방향키 왼쪽 오른쪽 처리
        else if (c == KEY_ARROW_LEFT)
        {
            int x,y;
            _GetCursor(&x, &y);
            if((x - 1) >= Prompt_length)
            {
                _SetCursor(x - 1, y);
            }
        }
        else if(c == KEY_ARROW_RIGHT)
        {
            int x,y;
            _GetCursor(&x, &y);
            if((x + 1) <= CommandBufferIndex)
            {
                _SetCursor(x + 1, y);
            }
        }
        else
        {
            if(c == KEY_TAB)
                c = ' ';
            if(CommandBufferIndex < SHELL_INPUT_BUFFER_SIZE)
            {
                CommandBuffer[CommandBufferIndex ++ ] = c;
                _Printf("%c", c);
            }
        }
	}    
}
//명령어, 파라미터 입력받아 처리하는 함수
void ExecuteCommand(const char * CommandBuffer)
{
    int command_index = 0;
    const int length = __StringLength(CommandBuffer);
    //커멘드 분리
    for(command_index = 0; command_index < length; command_index++)
    {
        if(CommandBuffer[command_index] == ' ')
            break;
    }

    const int CommandTableSize = sizeof(g_ShellCommandTable)/sizeof(SHELLCOMMAND);

    for(int i = 0; i <CommandTableSize; i++)
    {
        int command_length = __StringLength(g_ShellCommandTable[i].Command);
        //커멘드 길이와 안의 요소가 같다면
        if((command_length == command_index) && (_MemCmp(g_ShellCommandTable[i].Command, CommandBuffer, command_index) == 0))
        {
            //파라미터 문자열부터 전달
            g_ShellCommandTable[i].command_callback(CommandBuffer + command_index + 1);
            return;
        } 
    }
      _Printf("\'%s\' command not found\n",CommandBuffer);  

}


void Clear()
{
    CHARACTER_MEMORY* console = (CHARACTER_MEMORY*) CONSOLE_VIDEO_MEMORY;
    for(int i = 0 ;  i < CONSOLE_WIDTH * CONSOLE_HEIGHT; i ++)
    {
        console[i].bCharactor = ' ';
    }
    _SetCursor(0,0);
}

void SetAttribute(BYTE _attribute)
{
    CONSOLESYSTEM cons =  __GetConsole_System();
    cons.current_attribute  = _attribute;
    __SetConsole_System(cons);

    CHARACTER_MEMORY* console = (CHARACTER_MEMORY*) CONSOLE_VIDEO_MEMORY;
    for(int i = 0 ;  i < CONSOLE_WIDTH * CONSOLE_HEIGHT; i ++)
    {
        console[i].bAttribute = _attribute;
    }
}
//파라미터 문자열을 파라미터 리스트에 초기화
void InitializeParameter(PARAMETERLIST* _List, const char* _Parameter)
{
    _List->Buffer = _Parameter;
    _List->Length = __StringLength(_Parameter);
    _List->CurrentPosition = 0; 
}   
//다음 파라미터의 문자열을 포인터로 전달하고, 그 파라미터 길이 반환(없으면 0)
int GetNextParameter(PARAMETERLIST* _List, char* _Parameter_Out)
{
    int param_length = 0;
    if(_List->Length <= _List->CurrentPosition)
        return 0;

    int i = 0;
    //파라미터 구분
    for(i = _List->CurrentPosition; i < _List->Length; i++)
    {
        if(_List->Buffer[i] == ' ')
         {
            break;     
         }   
    }

    _MemCpy(_Parameter_Out, _List->Buffer + _List->CurrentPosition, i);
    param_length = i - _List->CurrentPosition;
    _Parameter_Out[param_length] = '\0';
    _List->CurrentPosition += param_length + 1;
    return param_length;
}

void Command_Help(const char* _Parameter)
{
    _Printf("\n");
    _Printf("============================= Help ===========================\n");

    const int command_table_size = sizeof(g_ShellCommandTable) / sizeof(SHELLCOMMAND);

    int max_command_length = 0;

    for(int i = 0; i <command_table_size; i ++)
    {
        int length = __StringLength(g_ShellCommandTable[i].Command);
        if(length > max_command_length)
            max_command_length = length;
    }

    int x,y;
    for(int i = 0; i < command_table_size; i++)
    {
        _Printf("%s", g_ShellCommandTable[i].Command);
        _GetCursor(&x, &y);
        _SetCursor(max_command_length, y);
        _Printf(" : %s\n", g_ShellCommandTable[i].Comment);
    }

    _Printf("============================= Help ===========================\n");
    _Printf("\n");
    
}
void Command_Clear(const char* _Parameter)
{

    char buf[200];
    PARAMETERLIST list;
    InitializeParameter(&list, _Parameter);
    BOOL IsSetForeground = FALSE;
    BOOL IsSetBackGround = FALSE;
    BYTE attribute = 0;
    while(1)
    {
        int length = GetNextParameter(&list, buf);
        if(length==0)
            break;
        if(_MemCmp(buf, "-f", 2) == 0)
        {
            int length = GetNextParameter(&list, buf);
            if(length==0)
                break;
            if(_MemCmp(buf, "green", 5) == 0)
            {
                attribute |= CONSOLE_FOREGROUND_GREEN;
            }
            else if(_MemCmp(buf, "white", 5) == 0)
            {
               attribute |= CONSOLE_FOREGROUND_WHITE;         
            }
            else if(_MemCmp(buf, "cyan", 4) == 0)
            {
               attribute |= CONSOLE_FOREGROUND_CYAN;         
            }
            else if(_MemCmp(buf, "black", 5) == 0)
            {
               attribute |= CONSOLE_FOREGROUND_BLACK;         
            }
            else
            {
                _Printf("Not Match Parameter\n");
                return;
            }
            IsSetForeground = TRUE;
        }

        if(_MemCmp(buf, "-b", 2) == 0)
        {
            int length = GetNextParameter(&list, buf);
            if(length==0)
                break;
            if(_MemCmp(buf, "green", 5) == 0)
            {
                attribute |= CONSOLE_BACKGROUND_GREEN;
            }
            else if(_MemCmp(buf, "white", 5) == 0)
            {
               attribute |= CONSOLE_BACKGROUND_WHITE;         
            }
            else if(_MemCmp(buf, "cyan", 4) == 0)
            {
               attribute |= CONSOLE_BACKGROUND_CYAN;         
            }
            else if(_MemCmp(buf, "black", 5) == 0)
            {
               attribute |= CONSOLE_BACKGROUND_BLACK;         
            }
            else
            {
                _Printf("Not Match Parameter\n");
                return;
            }
            IsSetBackGround = TRUE;
        }
    }
    BYTE current_attribute = __GetConsole_System().current_attribute;
    if(IsSetBackGround == FALSE)
        attribute |= (current_attribute & 0xF0);
    if(IsSetForeground == FALSE)
        attribute |= (current_attribute & 0x0F);
    
    SetAttribute(attribute);
    Clear();   


}

void Command_ShutDown(const char* _parameter)
{
    _Printf("\n Now Shutdown PC Press Any Key....\n");
    _GetCh();
    Reboot();
}

void Command_TotalRamSize(const char* _Parameter)
{
    _Printf("Ram Size = %d MB\n", __GetTotalRamSize());
}

void Command_StringToNumber(const char* _Parameter)
{
    char Parambuffer[200];

    PARAMETERLIST list;
    InitializeParameter(&list, _Parameter);
    int count = 0;
    while(1)
    {
        int length = GetNextParameter(&list, Parambuffer);

        if(length==0)
            break;
        
        _Printf("Parameter %d = %s, Length = %d \n", count + 1, Parambuffer, length);

        if(_MemCmp(Parambuffer, "0x", 2) == 0)
        {
            long value = 0;
            if(_atoi(Parambuffer + 2, &value, 16))
                _Printf("Hex Value = %d\n", value);
            else
                _Printf("Error\n");
        }
        else
        {
           long value = 0;
            if(_atoi(Parambuffer, &value, 10))
                _Printf("Value = %d\n", value);
            else
                _Printf("Error\n");
        }
        count++;
    }
}




void Command_SetTimer(const char* _Parameter)
{
    char ParameterBuffer[200];
    PARAMETERLIST pList;
    InitializeParameter(&pList, _Parameter);

    if(GetNextParameter(&pList,ParameterBuffer) == 0)
    {
        _Printf("settimer {time(ms)} {interval}\n");
        return;
    }
    long value;
    if(_atoi(ParameterBuffer, &value, 10) == FALSE)
    {
        _Printf("Parameter Decimal number\n");
        return;
    }

    if(GetNextParameter(&pList,ParameterBuffer) == 0)
    {
        _Printf("settimer {time(ms)} {interval}\n");
        return;
    }

    long interval_value;
    if(_atoi(ParameterBuffer, &interval_value, 10) == FALSE)
    {
        _Printf("Parameter is not Decimal number\n");
        return;
    }
    
    InitializePIT(MS_TO_COUNT(value), interval_value != 0);
    _Printf("Time = %d ms. Interval = %s Change Complate\n",value, (interval_value == 0)? "False": "True"); 

}
void Command_PITWait(const char* _Parameter)
{
    char ParameterBuffer[200];
    PARAMETERLIST pList;
    InitializeParameter(&pList, _Parameter);
    if(GetNextParameter(&pList,ParameterBuffer)==0)
    {
        _Printf("wait {time(ms)}\n");
        return;
    }
    
    long value;
    if(_atoi(ParameterBuffer, &value, 10) == FALSE)
    {
        _Printf("Parameter is not Decimal number\n");
        return;
    }

    _Printf("%d ms Sleep Start...\n", value);

    DisableInterrupt();

    for(long i =0; i < value/30L; i++)
    {
        WaitUsingPITCounter0(MS_TO_COUNT(30));
    }

    WaitUsingPITCounter0(MS_TO_COUNT(value % 30));
    
    
    EnableInterrupt();
    _Printf("%d ms Sleep Complate.\n", value);
    InitializePIT(MS_TO_COUNT(1), TRUE); 
}
void Command_ReadTimeStamp(const char* _Parameter)
{
    QWORD tsc =  ReadTSC();
    _Printf("Time Stamp Counter = %q \n",tsc);
}
void Command_CPUSpeed(const char* _Parameter)
{
    QWORD last_tsc;
    QWORD total_tsc = 0;
    _Printf("Now Calculate.");
    DisableInterrupt();
    for(int i = 0; i < 200; i++)
    {
        last_tsc = ReadTSC();
        WaitUsingPITCounter0(MS_TO_COUNT(50));
        total_tsc+= ReadTSC() - last_tsc;
        _Printf(".");
    }
    InitializePIT(MS_TO_COUNT(1),TRUE);
    EnableInterrupt();
    _Printf("\n Cpu Clock = %d MHz \n", total_tsc/10/1000/1000);
}
void Command_ShowDateTime(const char* _Parameter)
{
    BYTE Second, Minute, Hour;
    BYTE DayOfWeek, DayOfMonth, Month;
    WORD Year;

    ReadRTCTime(&Hour, &Minute, &Second);
    ReadRTCDate(&Year,&Month,&DayOfMonth, &DayOfWeek);

    _Printf("Date: %d-%d-%d %s, ", Year, Month, DayOfMonth, ConvertDayOfWeekString(DayOfWeek));
    _Printf("Time: %d:%d:%d\n", Hour, Minute, Second);
    

}

