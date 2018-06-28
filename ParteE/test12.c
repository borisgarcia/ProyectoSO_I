int main()
{
  enableInterrupts();
  syscall_printString("Hola");
  syscall_terminateProgram();
}

