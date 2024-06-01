#include <n7OS/syscall_defs.h>

fn_ptr syscall_table[NB_SYSCALL];

char *syscall_names[NB_SYSCALL];

void add_syscall(int num, fn_ptr function, char *name)
{
  if (num < NB_SYSCALL)
  {
    syscall_table[num] = function;
    syscall_names[num] = name;
  }
}

void print_syscall_table()
{
  for (int i = 0; i < NB_SYSCALL; i++)
  {
    printf("syscall_table[%d] = %s\n", i, syscall_names[i]);
  }
}

int syscall_id(char *name, int len)
{
  for (int i = 0; i < NB_SYSCALL; i++)
  {
    if (((int)strlen(syscall_names[i]) == len) && strcmpk(name, syscall_names[i], len) == 0)
    {
      return i;
    }
  }
  return -1;
}

fn_ptr syscall_function(char *name, int len)
{
  int id = syscall_id(name, len);
  if (id != -1)
  {
    return syscall_table[id];
  }
  return 0;
}

// call a syscall by its name
int syscall(char *name, int len)
{
  fn_ptr function = syscall_function(name, len);
  if (function != 0)
  {
    function();
    return syscall_id(name, len);
  }
  return -1;
}

int strcmpk(const char *s1, const char *s2, int n)
{
  for (int i = 1; i < n; i++)
  {
    if (s1[i] != s2[i])
    {
      return 1;
    }
  }
  return 0;
}