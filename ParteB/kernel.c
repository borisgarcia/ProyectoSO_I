#ifndef _KERNEL_C
#define _KERNEL_C

void printString(char*);
void readString(char []);
void interrupt21ServiceRoutine(int,int,int);

int main()
{
  makeInterrupt21();
  loadProgram();
  return 0;
}

void printString(char * mensaje)
{
  char *m = mensaje;
  while(*m != '\0')
    printChar(*m++);

}

void readString(char linea[])
{
  int n=0;
  char _char;
  while(_char != 0xd && n < 80)
  {
    _char = readChar();
    
    
    if(_char == 0x8)
    {
      if(n == 0)
        n = n - 1;
      else{
        printChar(_char);
        printChar(' ');
        printChar(0x8);
        n = n - 2;
      }
    }
    else
    {
      printChar(_char);
      linea[n] = _char;
      
    }    
    n++;
  }

  printChar('\n');
  linea[n] = '\0';
}

void handleInterrupt21(int ax,int bx,int cx)
{
    if (ax == 0)
        printString(bx);
    else if (ax == 1)
        readString(bx);
    else if (ax == 2)
        readSector(bx, cx);
    else
      printString("Error");
}

#endif