
#ifndef __SHELL_H__
#define __SHELL_H__


#include <Types.h>
#include <Driver/Time/PIT.h>
#include <Driver/Time/RTC.h>
#include <Driver/Time/TSC.h>

#define SHELL_INPUT_BUFFER_SIZE 500
#define SHELL_PROMPT_MESSAGE    "0SOS>"

typedef void (*CommandCallBack) (const char* parameter);

#pragma pack(push, 1)
typedef struct __Struct_ShellCommandEntry
{
    char* Command;
    char* Comment;
    CommandCallBack command_callback;
} SHELLCOMMAND;

typedef struct __Struct_Shell_Parameters
{
    const char* Buffer;
    int Length;
    int CurrentPosition;
} PARAMETERLIST;

#pragma pack(pop)


//--------------------------------------------------- *
void Command_Help(const char* _Parameter);
void Command_Clear(const char* _Parameter);
void Command_ShutDown(const char* _Parameter);
void Command_TotalRamSize(const char* _Parameter);
void Command_StringToNumber(const char* _Parameter);
void Command_SetTimer(const char* _Parameter);
void Command_PITWait(const char* _Parameter);
void Command_ReadTimeStamp(const char* _Parameter);
void Command_CPUSpeed(const char* _Parameter);
void Command_ShowDateTime(const char* _Parameter);
//---------------------------------------------------*
static SHELLCOMMAND g_ShellCommandTable[] = { 
    {"clear", "clear the consol\n-f {white, green, cyan,black} front color\n-b {black, white, blue} back ground\n"
    , Command_Clear},
    {"help", "help for 0SOS", Command_Help},
    {"shutdown", "Shutdown PC", Command_ShutDown},
    {"strtod", "String To Hex or Decimal", Command_StringToNumber},
    {"totalram", "Show Ram Size", Command_TotalRamSize},
    {"settimer", "Set PIT Controller Counter0.\nsettimer {time(ms)} {interval}\n", Command_SetTimer},
    {"wait", "Wait ms Use PIT. \n wait {time(ms)} \n", Command_PITWait},
    {"rdtsc", "Read Time Stamp Counter", Command_ReadTimeStamp},
    {"cpuspd", "CPU Speed", Command_CPUSpeed},
    {"date", "Show DateTime", Command_ShowDateTime}
};

void Clear();
void SetAttribute(BYTE _attribute);
void StartShell();
void ExecuteCommand(const char * CommandBuffer);
void InitializeParameter(PARAMETERLIST* _List, const char* _Parameter);
int GetNextParameter(PARAMETERLIST* _List, char* _Parameter_Out);






#endif /*__SHELL_H__*/