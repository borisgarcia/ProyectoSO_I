void set_cursor(int, int);
void put_char(char);
void puts(char*);
void clear_screen();
int col,row;

int main()
{
  clear_screen();
  set_cursor(1,30);
  puts("Sistemas Operativos I");
  set_cursor(2,16);
  puts("Universidad Tecnologica Centroamericana (UNITEC)");
  set_cursor(3,30);
  puts("21521082 - Boris Garcia");
  set_cursor(4,26);
  puts("Mi Primer Sistema Operativo");
  set_cursor(5,1);
  puts("--------------------------------------------------------------------------------");
  puts("-----------------------------------Hola Mundo-----------------------------------");
  puts("-----------------------------------Hello World----------------------------------");
  puts("----------------------------------Pozdrav svijet--------------------------------");
  puts("-----------------------------------Hej Verden-----------------------------------");
  puts("------------------------------------ahoj svet-----------------------------------");
  puts("---------------------------------Bonjour le monde-------------------------------");
  puts("------------------------------------Helo Byd------------------------------------");
  puts("-----------------------------------hola hello-----------------------------------");
  puts("---------------------------------Dia duit domhan--------------------------------");
  puts("-----------------------------------Ciao mondo-----------------------------------");
  puts("----------------------------------Witaj swiecie---------------------------------");
  puts("-----------------------------------Hej varlden----------------------------------");
  puts("-----------------------------------Ola Mundo------------------------------------");
  puts("-----------------------------------Hei Verden-----------------------------------");
  puts("-----------------------------------Halo Dunia-----------------------------------");
  puts("-----------------------------------Ahoj svete-----------------------------------");
  puts("-----------------------------------Hallo Welt-----------------------------------");
  puts("----------------------------------Hallo heimur----------------------------------");
  puts("-----------------------------------Hei maailma-----------------------------------");


  while(1)
  {

  }
  return 0;
}

void set_cursor(int r, int c)
{
  if(r > 24 || c > 79)
    return;

  row = r;
  col = c;
}

void put_char(char _char)
{
  if(_char == '\n')
  {
    if(row < 25)
      row++;
    col = 0;
  }

  else{
    int p = 0x8000 + (80*row+col)*2;
    putInMemory(0xB000,p,_char);
    putInMemory(0xB000,p+1,0x3);

    if(col < 79)
      col++;
    else
    {
      col = 0;
      row++;
    }
  }
}

void puts(char * mensaje)
{
  char *m = mensaje;
  while(*m != '\0')
    put_char(*m++);
}

void clear_screen()
{
  int i;
  for(i= 0x8000; i <= 0x8fa0; i=i+2)
    putInMemory(0xB000,i,' ');
} 