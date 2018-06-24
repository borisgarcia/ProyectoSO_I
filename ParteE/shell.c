int isEqual(char[], char[]);
void split(int,char[],char[]);
void cleanBuffer(char*);
int main()
{
    char str[80];
    char buffer[13312];
    enableInterrupts();
    syscall_printString("Project C: ");
    syscall_printString("\r\n");
    syscall_printString("\r\n");
    
    while(1)
    {
        syscall_printString("SHELL> ");
        syscall_readString(str);
        cleanBuffer(buffer);
        //cleanBuffer(str);
        if(isEqual("execute",str))
        {
            syscall_executeProgram(&str[8],0x4000);
            syscall_printString("\r\n");
        }
        
        else if(isEqual("type",str))
        {
            syscall_readFile(&str[5],buffer);
            syscall_printString("\r\n");
            syscall_printString(buffer);
            syscall_printString("\r\n");
        }

        else if(isEqual("delete",str))
        {
            syscall_deleteFile(&str[7]);
        }

        else if(isEqual("copy",str))
        {
            syscall_copyFile(&str[5],&str[12]);
            syscall_printString("\r\n");
        }

        else if(isEqual("clear",str))
            syscall_clearScreen();

        else if(isEqual("exit",str))
        {
            syscall_terminateProgram();
            syscall_printString("\r\n");

        }
        else if(isEqual("dir",str))
        {
            syscall_dir();
            syscall_printString("\r\n");
        }

        else if(isEqual("create",str))
        {
            syscall_printString("Escriba: ");
            syscall_createTXT(&str[7]);
            syscall_printString("\r\n");
        } 

        else if(isEqual("kill",str))
        {
            syscall_printString("Killing: ");
            syscall_createTXT(&str[5]);
            syscall_killProcess("\r\n");
        }            

        else
        {
            syscall_printString("Command not Found!");
            syscall_printString("\r\n");
        }
            
    }
}


int isEqual(char buffS[], char str[])
{
    int x;
    
    for (x = 0; buffS[x] != '\0' && str[x] != '\0'; x++)
    {
        if (buffS[x] != str[x])
	        return 0;
    }
    return 1;
}

void cleanBuffer(char buffer[])
{
    while(*buffer != '\0')
    {
        *buffer = 0x00;
        *buffer++;        
    }
}