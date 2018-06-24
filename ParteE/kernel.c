void printString(char*);
void readString(char []);
void handleInterrupt21(int,int,int,int);
void readFile(char *,char[]);
void terminateProgram();
void executeProgram(char *);
void clear_screen();
int searchFile(char*);
void deleteFile(char*); 
void writeFile(char*, char*, int);
void copyFile(char *, char *);
void createTXT(char*);
void dir();
void handleTimerInterrupt(int, int);
void killProcess(int);

int activeProcess[8];
int processSP[8];
int currentProcess;
int timeU = 0;
int firstExecution = 0;

int main()
{
  int x;
  for(x=0;x<8;x++)
  {
    activeProcess[x]=0;
    processSP[x]= 0xFF00;
  }
  currentProcess = 0;
  makeInterrupt21();
  executeProgram("shell");
  makeTimerInterrupt();
  while(1);
  return 0;
}


void terminateProgram()
{
  setKernelDataSegment();
  activeProcess[currentProcess] = 0;
	processSP[currentProcess] = 0xFF00;
  while(1);
}

void executeProgram(char *name)
{
  int cBuffer = 0;
  char buffer[13312];
  int x,segment;

  setKernelDataSegment();
  for(x = 0;x<8;x++)
  {
    if(activeProcess[x] == 0)
    {
      activeProcess[x] = 1;
      restoreDataSegment();
      segment = (x+2)*0x1000;
      break;
    }
  }  
    


  readFile(name, buffer);
  
  for (; cBuffer < 13312; cBuffer++)
    putInMemory(segment, cBuffer, buffer[cBuffer]);

  initializeProgram(segment);
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

void deleteFile(char* name)
{
	char directory [512];
	char map [512];
	int found,i;

	readSector(map,1);
	readSector(directory,2);
	
  found = searchFile(name);

  if(found != -1)
  {
    directory[found*32] == 0x00;
    for (i=6;i<32;i++)
    {
      directory[(found*32)+i] = 0x00;
      map[directory[(found*32)+i]] = 0x00;
    }
  }
  else
  {
    return;
  }
  writeSector(map,1);
  writeSector(directory,2);
}

void dir()
{
  char fileName[6],subBuf[6];
  char directory [512];
  int x,y,i;
  readSector(directory,2);
  printChar('H');
  for(x=0;x<16;x++)
  {
		for(y=0;y<6;y++)
    {
			if(directory[(x*32)+y]!=0x00)
        fileName[y] = directory[(x*32)+y];
        
      else
        break;
      
      if(y == 5)
      {
        for(i=0;i<6;i++)
          printChar(fileName[i]);
        printChar(' ');
      }
		}
	}
}

void writeFile(char* name, char* buffer, int numberOfSectors)
{
  char directory [512];
	char map [512];
  int exists = -1,freeSector = -1,x,y,i,j,a=0;
  readSector(map,1);
	readSector(directory,2);  
  
  exists = searchFile(name);
  if(exists !=-1)
  {
    return;
  }

  //Al encontrar un espacio vacio se guarda el numero en freeSector
  for(x=0;x<16;x++)
  {
    if (directory[32*x] == 0x00 && freeSector == -1)
    {
      freeSector = x;
      break;
    }   
	}

  //Si se encontro un espacio vacio
  if(freeSector != -1)
  {
    
    //Escribimos el nombre del File en el Directory
    for(i = 0;i<6;i++)
    {
      if(name[i] != '\0')
        directory[(freeSector*32)+i] = name[i];
      else
        directory[(freeSector*32)+i] = 0x00;
    }

    for (j = 3; numberOfSectors != 0 && j<512; j++)
    {
      if(map[j] == 0x00)
      {
        map[j] = 0xFF;
        directory[(freeSector*32)+i]=j;
        writeSector(buffer+(a*512),j);
        numberOfSectors--;
        i++;
        a++;
      }
    }

    for(;i<32;i++)
      directory[(freeSector*32)+i]=0x00;
    
    writeSector(map,1);
    writeSector(directory,2);
  }
}

void copyFile(char * filename1,char * filename2)
{
  int numSector,location,x;
  char directory[512];
  char buffer[13312];
  
  location = searchFile(filename1);
  if(location == -1)
  {
    return;
  }
  
  readSector(directory,2);

  for(x = 6;x<32;x++)
  {
    if(directory[(location*32)+x]!=0x00)
      numSector++;
  }
  readFile(filename1,buffer);
  writeFile(filename2,buffer,numSector);

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

void createTXT(char* filename)
{
  char buffer[13312];
  char line[80];
  int x,y;
  
  line[0]='-';

  if(searchFile(filename)!=-1)
    return;

  while(line[0]!=0 && x < 13312)
  {
    printChar('\0');
    readString(line);
    for(y=0;y<80 && x<13312;y++)
    {
      if(line[y]==0)
        break;
      buffer[x++]=line[y];
    }
    buffer[x++]= '\r';
    buffer[x++]= '\n';
  }
  buffer[x] = '\0';
  writeFile(filename,buffer,(x/512)+1);
}

void killProcess(int id)
{
	activeProcess[id] = 0;
	processSP[id] = 0xFF00;
}

void handleTimerInterrupt(int segment, int sp){
	int x;

	if(firstExecution==0)
  {
		timeU++;
    firstExecution = 1;
		returnFromTimer(0x2000, processSP[0]);
	}
	else
  {
    x = currentProcess + 1;
    if(timeU == 100)
    {
      timeU = 0;
      processSP[currentProcess] = sp;
      while(1)
      {
        if((x&8)==currentProcess)
          break;
        if(activeProcess[x]==1)
        {
          segment = ((x&8)+1) * 0x1000;
          sp = processSP[x];
          currentProcess = x;
          break;
        }
        x++;
      }
    }
    else
      timeU++;
	  returnFromTimer(segment, sp);
  }	
}

void handleInterrupt21(int ax,int bx,int cx,int dx)
{
  if (ax == 0)
    printString(bx);
  else if (ax == 1)
    readString(bx);
  else if (ax == 2)
    readSector(bx,cx);
  else if(ax == 3)
    readFile(bx,cx);
  else if(ax == 4)
    executeProgram(bx);
  else if(ax == 5)
    terminateProgram();
  else if(ax == 6)
    writeSector(bx,cx);
  else if(ax == 7)
    deleteFile(bx);
  else if(ax == 8)
    writeFile(bx,cx,dx);
  else if(ax == 9)
    killProcess(bx);
  else if(ax == 10)
    clear_screen();
  else if(ax == 11)
    copyFile(bx,cx);
  else if(ax == 12)
    createTXT(bx);
  else if(ax == 13)
    dir();
}

void clear_screen()
{
  int i;
  for(i= 0x8000; i <= 0x8fa0; i=i+2)
    putInMemory(0xB000,i,' ');
  interrupt(0x10,0x2,0x0,0x0,0x0);
}

