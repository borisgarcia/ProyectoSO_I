void printString(char*);
void readString(char []);
void interrupt21ServiceRoutine(int,int,int);
void readFile(char *,char[]);
void terminateProgram();
void executeProgram(char *, int);
void clear_screen();
int searchFile(char *);

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

void readFile(char * name,char buffer[])
{
	char directory[512];
	int location;
	int k=0;
	int j=6;
  readSector(directory,2);
  location = searchFile(name);

  if(location!=-1)
  {  
    while(directory[(location*32)+j] != 0x0 && k<26)
    {
      readSector(buffer+(k*512),directory[(location*32)+j]);
      j++;
      k++;
    }
  }
  else
    return;
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

int searchFile(char * name)
{
  char directory[512];
  int x,y,found;
  readSector(directory,2);

  for(x=0;x<16;x++)
  {
		for(y=0;y<6;y++)
    {
			if(directory[(x*32)+y]==name[y])
			  found = 1;
			else
      {
        found = -1;
        break;
		  }
		}
		if(found == 1)
      return x;
	}
  return -1;
}