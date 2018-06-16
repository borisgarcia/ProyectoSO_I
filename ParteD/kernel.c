void printString(char*);
void readString(char []);
void interrupt21ServiceRoutine(int,int,int);
void readFile(char *,char[]);
void terminateProgram();
void executeProgram(char *, int);
void clear_screen();

int main()
{
  makeInterrupt21();
  executeProgram("shell", 0x2000);
  while(1);

 
  return 0;
}

void terminateProgram()
{
  while(1);
}

void readFile(char *name, char buffer[])
{
    int next = 0,equal = 0,result = 20;
    int x,y,i,increment,sector,first;
    int dir[27];
    char directory[512];
    
    readSector(directory, 2);

    for (x = 0; x < 16; x++)
    {
        for (y = 0; y < 6; y++)
            if (directory[y + next] != name[y])
            {
              equal = 1;
              break;
            }
        
        next += 32;

        if (equal == 0)
            result = x;

        if (result != 20)
            break;
    }

    if (result == 20)
      return;

    else
    {
      first = result * 32 + 6;     
      for (i = 0; i < 26; i++)
        dir[i] = directory[first + i];
        dir[26] = 0;
        increment = 0;
        for (sector = 0; dir[sector] != 0x0; sector++)
        {
          readSector(buffer + increment, dir[sector]);
          increment += 512;
        }
    }
}

void executeProgram(char *name, int segment)
{
    int cBuffer = 0;
    char buffer[13312];

    readFile(name, buffer);
    
    for (; cBuffer < 13312; cBuffer++)
        putInMemory(segment, cBuffer, buffer[cBuffer]);
    
    launchProgram(segment);
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
      else
      {
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
  else if(ax == 3)
    readFile(bx,cx);
  else if(ax == 4)
    executeProgram(bx,cx);
  else if(ax == 5)
    terminateProgram();
  else if(ax == 10)
    clear_screen();
  else
    printString("Error");
}

void clear_screen()
{
  int i;
  for(i= 0x8000; i <= 0x8fa0; i=i+2)
    putInMemory(0xB000,i,' ');
  interrupt(0x10,0x2,0x0,0x0,0x0);
} 
