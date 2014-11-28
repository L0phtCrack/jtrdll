#ifndef __INC_DLLIO_H
#define __INC_DLLIO_H

#include<stdio.h>

int dllprintf(const char * _Format, ...);
int dllfprintf(FILE *f, const char * _Format, ...);
int dllfputs(const char *str, FILE *f);
int dllputs(const char *str);
int dllputchar(int c);
int dllfputc(int c, FILE *f);

#ifdef _CRT_TERMINATE_DEFINED
#undef _CRT_TERMINATE_DEFINED
#endif

#define _CRT_TERMINATE_DEFINED 1
__declspec(noreturn) void dllexit(int exitcode);
__declspec(noreturn) void dll_exit(int exitcode);
int dllsetexithook(void);

#define printf dllprintf
#define fprintf dllfprintf
#define fputs dllfputs
#define puts dllputs
#define putchar dllputchar
#define fputc dllfputc
#define _exit dllexit
#define exit dll_exit

extern FILE *dllstdout;
extern FILE *dllstderr;
extern int dllexitcode;


#endif