bcc -ansi -c -o test.o test.c
as86 -o os_api.o os_api.asm
ld86 -o test11 -d test.o os_api.o


