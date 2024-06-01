#include <n7OS/sys.h>

extern void handler_syscall();

void init_syscall()
{
  // ajout de la fonction de traitement de l'appel systeme
  add_syscall(NR_example, sys_example, "example");
  add_syscall(NR_shutdown, sys_shutdown, "shutdown");
  add_syscall(NR_write, sys_write, "write");
  add_syscall(NR_snake, sys_snake, "snake");
  add_syscall(NR_fork, (int (*)(const char *, void (*)(), int, char **))sys_fork, "fork");
  add_syscall(NR_clear, sys_clear, "clear");
  add_syscall(NR_print_memory, sys_print_memory, "print_memory");
  add_syscall(NR_help, sys_help, "help");
  add_syscall(NR_process, sys_process_test, "test_process");
  add_syscall(NR_ps, sys_ps, "ps");

  // initialisation de l'IT soft qui gère les appels systeme
  init_irq_entry(0x80, (uint32_t)handler_syscall);
  // print_syscall_table();
}

// code de la fonction de traitement de l'appel systeme example
int sys_example()
{
  // on ne fait que retourner 1
  return 1;
}
/*
int sys_shutdown(int n)
{
  if (n == 1)
  {
    outw(0x2000, 0x604); // Power off qemu > 2.0
    return -1;
  }
  else
  {
    return n;
  }
}*/

int sys_shutdown()
{
  outw(0x2000, 0x604); // Power off qemu > 2.0
  return -1;
}

int sys_write(const char *s, int len)
{
  int writtenBytes;
  if ((len >= 0))
  {
    console_putbytes(s, len);
    writtenBytes = len;
  }
  else
  {
    writtenBytes = -1;
  }
  return writtenBytes;
}

int sys_snake()
{
  sti();
  init_game();
  return 0;
}

int sys_clear()
{
  console_clear();
  return 0;
}

pid_t sys_fork(const char *name, fnptr f, int argc, char *argv[])
{
  return creer(name, f, argc, argv);
}

int sys_print_memory()
{
  print_mem();
  return 0;
}

int sys_help()
{
  print_help();
  return 0;
}

int sys_process_test()
{
  process_test();
  return 0;
}

int sys_ps()
{
  print_process();
  return 0;
}

/*

// code de la fonction de traitement de l'appel systeme write
int sys_write(int fd, const char *buf, size_t count) {
  // on appelle la fonction de la libc
  syscall3(int, write, int, fd, const char *, buf, size_t, count);
}
*/
