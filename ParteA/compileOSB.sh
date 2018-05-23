bcc -ansi -c -o test.o test.c
as86 -o os_api.o os_api.asm
ld86 -o syscall-test -d test.o os_api.o
dd if=syscall-test of=floppya.img bs=512 count=1 seek=11 conv=notrunc
