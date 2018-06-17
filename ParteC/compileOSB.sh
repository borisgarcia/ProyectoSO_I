bcc -ansi -c -o shell.o shell.c
as86 -o os_api.o os_api.asm
ld86 -o shell -d shell.o os_api.o


