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
            split(8,str,bOut);
            syscall_executeProgram(&bOut[8],0x4000);
            syscall_printString("\r\n");
        }
        
        else if(isEqual("type",str))
        {
            split(5,str,bOut);
            syscall_readFile(&bOut[5],buffer);
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

void split(int pos, char buffIn[],char buffOut[])
{
    int x;
    int pos = 0;
    for (x = 0; buffIn[x] != '\0' ; x++)
    {
        buffOut[pos] = buffIn[x];
	    pos++;
    }
    buffOut[pos] = '\0';
}