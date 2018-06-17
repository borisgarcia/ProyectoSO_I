int isEqual(char[], char[]);
void split(int,char[],char[]);

int main()
{
    char str[80];
    char bOut[25];
    char buffer[13312];
 
    syscall_printString("Project C: ");
    syscall_printString("\r\n");
    syscall_printString("\r\n");
    
    while(1)
    {
        syscall_printString("SHELL> ");
        syscall_readString(str);

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

        else if(isEqual("clear",str))
            syscall_clearScreen();

        else if(isEqual("exit",str))
            syscall_terminateProgram();

        else
            syscall_printString("Command not Found!");
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

