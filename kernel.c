#ifndef _KERNEL_C
#define _KERNEL_C

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
  executeProgram("shelll", 0x2000);
  while(1);
}

void terminateProgram()
{
  while(1);
}

void readFile(char *name, char buffer[])
{
    int next = 0;
    int first;
    int x,y,i;
    int sector;
    int equal = 0;
    int result = 20;
    int increment;
    int dir[27];
    char directory[512];
    
    readSector(directory, 2);

    for (x = 0; x < 16; x++)
    {
        for (y = 0; y < 6; y++)
        {
            if (directory[y + next] != name[y])
            {
              equal = 1;
              break;
            }
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
void readFile(char *name, char buffer[])
{
    int fileChanger = 0;
    int startingSector;
    int x,y;
    int sec;
    int notSameChar = 0;
    int result = 20;
    int n;
    int incrementerOfAddress;
    int dirSectors[27];
    char directory[512];
    
    readSector(directory, 2);

    //finding the file location
    for (x = 0; x < 16; x++)
    {
        for (y = 0; y < 6; y++)
        {
            if (directory[y + fileChanger] == name[y])
            {
                notSameChar = 0;
            }
            else

                notSameChar = 1;
            if (notSameChar == 1)
                break;
        }

        fileChanger += 32;

        if (notSameChar == 0)
            result = x;

        if (result != 20)
            break;
    }

    //if didn't found the file 
    if (result == 20)
    {
        return;
    }

    else
    {
        //load file sector by sector
        startingSector = result * 32 + 6;
        //(+6 , 6 for the next file name )
        for (n = 0; n < 26; n++)
        {
            dirSectors[n] = directory[startingSector + n];
        }
        dirSectors[26] = 0;
        incrementerOfAddress = 0;
        for (sec = 0; dirSectors[sec] != 0x0; sec++)
        {
            readSector(buffer + incrementerOfAddress, dirSectors[sec]);
            incrementerOfAddress += 512;
        }
    }
}

void executeProgram(char *name, int segment)
{
    int currentFromBuffer = 0;
    char buffer[13312];

    readFile(name, buffer);
    
    for (currentFromBuffer = 0; currentFromBuffer < 13312; currentFromBuffer++)
        putInMemory(segment, currentFromBuffer, buffer[currentFromBuffer]);
        
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
} 
#endif