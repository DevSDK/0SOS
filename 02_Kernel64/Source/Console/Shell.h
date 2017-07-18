#ifndef __SHELL_H__
#define __SHELL_H__

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
//---------------------------------------------------*
static SHELLCOMMAND g_ShellCommandTable[] = { 
    {"clear", "clear the consol", Command_Clear},
    {"help", "help for 0SOS", Command_Help},
    {"shutdown", "Shutdown PC", Command_ShutDown},
    {"strtod", "String To Hex or Decimal", Command_StringToNumber},
    {"totalram", "Show Ram Size", Command_TotalRamSize}
};

void Clear();
void StartShell();
void ExecuteCommand(const char * CommandBuffer);
void InitalizeParameter(PARAMETERLIST* _List, const char* _Parameter);
int GetNextParameter(PARAMETERLIST* _List, char* _Parameter_Out);






#endif /*__SHELL_H__*/