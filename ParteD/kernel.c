void printString(char*);
void readString(char []);
void handleInterrupt21(int,int,int,int);
void readFile(char *,char[]);
void terminateProgram();
void executeProgram(char *, int);
void clear_screen();
void writeSector(char*, int);
int Op(int,int,int);
void searchFile(char*,int);
void deleteFile(char*); 
void writeFile(char*, char*, int);
void copyFile(char *, char *);
void dir();

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

void writeSector(char* buffer, int sector)
{
  int number,AH,AL,AX,BX,CH,CL,CX,DH,DL,DX;
  AH = 3;
  AL = 1;
  BX = buffer;
  CH = Op(sector,36,1);
  CL = Op(sector,18,0) + 1;
  DH = Op(Op(sector,18,1),2,0);
  DL = 0;
  AX = AH*256+1;
  CX = CH*256+CL;
  DX = DH + 256;

  interrupt (0x13,AX,BX,CX,DX);
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

void deleteFile(char* name)
{
	char directory [512];
	char map [512];
	int found = 0,x,y,i;

	readSector(map,1);
	readSector(directory,2);
	
  for(x=0;x<16;x++)
  {
		for(y=0;y<6;y++)
    {
			if(directory[(x*32)+y]==name[y])
			  found = 1;
			else
      {
        found = 0;
        break;
		  }
		}
		if(found == 1)
    {
		  directory[(x*32)] = 0x00;
		  break;
	  }
	}

  if(found == 1)
  {
    for (i=6;i<32;i++)
    {
      directory[(x*32)+i] = 0x00;
      map[directory[(x*32)+i]] = 0x00;
    }
  }
  else
    printString("File not Found Bitch!");

  writeSector(map,1);
  writeSector(directory,2);
}

void dir()
{
  char fileName[6];
  char directory [512];
  int x,y;
  readSector(directory,2);

  for(x=0;x<16;x++)
  {
		for(y=0;y<6;y++)
    {
			if(directory[(x*32)+y]!=0x00)
      {
        fileName[y] = directory[(16*32)+y];
      }
        
      else
        break;
		}
    printString(fileName);
    printString(" - At Sector: ");
    printString(x);
    printString("\r\n");
	}
}

void writeFile(char* name, char* buffer, int numberOfSectors)
{
  char directory [512];
	char map [512];
  int exists = 0,freeSector = -1,x,y,i,j,a=0;
  readSector(map,1);
	readSector(directory,2);

  //Busca que no existe ningun File con ese nombre
  for(x=0;x<16;x++)
  {
    //Al encontrar un espacio vacio se guarda el numero en freeSector
    if (directory[32*x] == 0x00 && freeSector != -1)
      freeSector = x;

    //Se busca que no exista ningun File con el mismo nombre
		for(y=0;y<6;y++)
    {
			if(directory[(x*32)+y]==name[y])
			  exists = 1;
			else
      {
        exists = 0;
        break;
		  }
		}
		if(exists == 1)
    {
      //Si se encuentra un File con ese mismo nombre la funcion termina
		  printString("cannot create File: File exists");
		  return;
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

    for (j = 3; 0 != numberOfSectors && j<512; j++)
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
  char buffer[512];
  int numSector,location,x;
  char directory[512];
  
  searchFile(filename1,location);
  

  if(location == -1)
  {
    printString("File not Found Bitch!");
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

void searchFile(char * filename,int location)
{
  char directory[512];
  int x,y,found;
  readSector(directory,2);

  for(x=0;x<16;x++)
  {
		for(y=0;y<6;y++)
    {
			if(directory[(x*32)+y]==filename[y])
			  found = 1;
			else
      {
        found = 0;
        break;
		  }
		}
		if(found == 1)
    {
		  location = x;
		  break;
	  }
	}
  location = -1;
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
    executeProgram(bx,cx);
  else if(ax == 5)
    terminateProgram();
  else if(ax == 6)
    writeSector(bx,cx);
  else if(ax == 7)
    deleteFile(bx);
  else if(ax == 8)
    writeFile(bx,cx,dx);
  else if(ax == 9)
    dir();
  else if(ax == 10)
    clear_screen();
  else if(ax == 11)
    copyFile(bx,cx);
  //else if(ax == 12)
    //createTXT();
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

int Op(int x, int y,int _op)
{
  //Mod
  if(_op == 0)
  {
    while(x >= y)
        x = x - y;
    return x;
  }
  //Div
  else if(_op == 1)
  {
    int a = 0;
    while(a*x <= y)
        a++;
    return a-1;
  }
    
  return -1;
}