#ifndef _TEST_C
#define _TEST_C

void printString(char*);
void readString(char []);
void interrupt21ServiceRoutine(int,int,int);

int main()
{
  char str[80], buffer[512];

  printString("Leer:");
  printString("\r\n");
  readString(str);
  


  while(1)
  {

  }

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
    printChar(_char);
    
    if(_char == 0x8)
    {
      printChar(' ');
      printChar(0x8);
      n--;
    }
    else
    {
      linea[n] = _char;
      n++;
    }    
  }

  printChar('\n');
  linea[n] = '\0';
}

void  interrupt21ServiceRoutine(int ax,int bx,int cx)
{
    if (ax == 0)
        printString(bx);
    else if (ax == 1)
        readString(bx);
    //else if (ax == 2)
        //readSector(bx, cx);
    else if (ax == 3)
      printString("Error");
}

#endif