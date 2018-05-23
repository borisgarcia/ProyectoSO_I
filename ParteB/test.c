#ifndef _TEST_C
#define _TEST_C

int main()
{
  char str[80], buffer[512];
  
  syscall_printString("Enter a line: ");
  syscall_readString(str);
  
  syscall_printString(str);
  syscall_printString("\r\n");
  syscall_printString("Reading sector 30 from floppy\r\n");
  syscall_printString("\r\n");
  syscall_readSector(buffer, 30);
  syscall_printString(buffer);

  while(1){}

}

#endif