#include "Shell.h"
#include <Driver/Keyboard/Keyboard.h>
#include <Driver/VGA/IO_VGA.h>
#include <Utility/String.h>
#include <Console/Console.h>
void StartShell()
{
    __InitializeMemoryCheck();
    char CommandBuffer[SHELL_INPUT_BUFFER_SIZE];
    _SetCursor(0, 17);
    int CommandBufferIndex = 0;
    _Printf(SHELL_PROMPT_MESSAGE);
    const int Prompt_length = sizeof(SHELL_PROMPT_MESSAGE);
	while(1)
	{
       	BYTE c = _GetCh();

        if(c == KEY_BACKSPACE)
        {
            if(CommandBufferIndex > 0)
            {
                int x, y;
                _GetCursor(&x, &y);
                int dx = x - Prompt_length;   
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
                if(CommandBufferIndex == 0)
                {
                    _SetCursor(Prompt_length-1, y);                    
                }

            }
        }
        else if(c == KEY_ENTER)
        {
            _Printf("\n");
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

void ExecuteCommand(const char * CommandBuffer)
{
    int command_index = 0;
    const int length = __StringLength(CommandBuffer);
    for(command_index = 0; command_index < length; command_index++)
    {
        if(CommandBuffer[command_index] == ' ')
            break;
    }

    const int CommandTableSize = sizeof(g_ShellCommandTable)/sizeof(SHELLCOMMAND);

    for(int i = 0; i <CommandTableSize; i++)
    {
        int command_length = __StringLength(g_ShellCommandTable[i].Command);
        if((command_length == command_index) && (_MemCmp(g_ShellCommandTable[i].Command, CommandBuffer, command_index) == 0))
        {
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

void InitalizeParameter(PARAMETERLIST* _List, const char* _Parameter)
{
    _List->Buffer = _Parameter;
    _List->Length = __StringLength(_Parameter);
    _List->CurrentPosition = 0; 
}   
int GetNextParameter(PARAMETERLIST* _List, char* _Parameter_Out)
{
    int param_length = 0;
    if(_List->Length <= _List->CurrentPosition)
        return 0;

    int i = 0;
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

;

    InitalizeParameter(&list, _Parameter);
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