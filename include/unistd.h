#ifndef __UNISTD_H__
#define __UNISTD_H__

#define NR_example 0
#define NR_shutdown 1
#define NR_write 2
#define NR_snake 3
#define NR_fork 4
#define NR_clear 5
#define NR_print_memory 6
#define NR_help 7
#define NR_process 8
#define NR_ps 9

// Fonction d'enveloppe sans argument
#define syscall0(type, name)                \
	type name(void)                         \
	{                                       \
		type __res;                         \
		__asm__ volatile("int $0x80"        \
						 : "=a"(__res)      \
						 : "0"(NR_##name)); \
		return __res;                       \
	}

// Fonction d'enveloppe 1 argument
#define syscall1(type, name, atype, a)              \
	type name(atype a)                              \
	{                                               \
		type __res;                                 \
		__asm__ volatile("int $0x80"                \
						 : "=a"(__res)              \
						 : "0"(NR_##name), "b"(a)); \
		return __res;                               \
	}

// Fonction d'enveloppe 2 arguments
#define syscall2(type, name, atype, a, btype, b)            \
	type name(atype a, btype b)                             \
	{                                                       \
		type __res;                                         \
		__asm__ volatile("int $0x80"                        \
						 : "=a"(__res)                      \
						 : "0"(NR_##name), "b"(a), "c"(b)); \
		return __res;                                       \
	}

// Fonction d'enveloppe 3 arguments
#define syscall3(type, name, atype, a, btype, b, ctype, c)          \
	type name(atype a, btype b, ctype c)                            \
	{                                                               \
		type __res;                                                 \
		__asm__ volatile("int $0x80"                                \
						 : "=a"(__res)                              \
						 : "0"(NR_##name), "b"(a), "c"(b), "d"(c)); \
		return __res;                                               \
	}

#define syscall4(type, name, atype, a, btype, b, ctype, c, dtype, d)        \
	type name(atype a, btype b, ctype c, dtype d)                           \
	{                                                                       \
		type __res;                                                         \
		__asm__ volatile("int $0x80"                                        \
						 : "=a"(__res)                                      \
						 : "0"(NR_##name), "b"(a), "c"(b), "d"(c), "S"(d)); \
		return __res;                                                       \
	}

int example();
int shutdown(int n);
int write(const char *s, int len);
int snake();
int clear();

#endif
