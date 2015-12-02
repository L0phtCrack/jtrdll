#ifndef __INC_DLLIO_H
#define __INC_DLLIO_H

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wformat"
#endif

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

int dllprintf(const char * _Format, ...);
int dllfprintf(FILE *f, const char * _Format, ...);
int dllfputs(const char *str, FILE *f);
int dllputs(const char *str);
int dllputchar(int c);
int dllfputc(int c, FILE *f);

void * dllcalloc(size_t _Count, size_t _Size);
void   dllfree(void * _Memory);
void * dllmalloc(size_t _Size);
void * dllrealloc(void * _Memory, size_t _NewSize);
void * dll_recalloc(void * _Memory, size_t _Count, size_t _Size);
void   dll_aligned_free(void * _Memory);
void * dll_aligned_malloc(size_t _Size, size_t _Alignment);
void * dll_aligned_offset_malloc(size_t _Size, size_t _Alignment, size_t _Offset);
void * dll_aligned_realloc(void * _Memory, size_t _NewSize, size_t _Alignment);
void * dll_aligned_recalloc(void * _Memory, size_t _Count, size_t _Size, size_t _Alignment);
void * dll_aligned_offset_realloc(void * _Memory, size_t _NewSize, size_t _Alignment, size_t _Offset);
void * dll_aligned_offset_recalloc(void * _Memory, size_t _Count, size_t _Size, size_t _Alignment, size_t _Offset);

#ifdef _CRT_TERMINATE_DEFINED
#undef _CRT_TERMINATE_DEFINED
#endif

#define _CRT_TERMINATE_DEFINED 1

#ifdef _MSC_VER
__declspec(noreturn) void dllexit(int exitcode);
__declspec(noreturn) void dll_exit(int exitcode);
#else
void dllexit(int exitcode) __attribute__ ((noreturn));
void dll_exit(int exitcode) __attribute__ ((noreturn));
#endif

int dllsetexithook(void);

#define printf dllprintf
#define fprintf dllfprintf
#define fputs dllfputs
#define puts dllputs
#define putchar dllputchar
#define fputc dllfputc
#define _exit dllexit
#define exit dll_exit
#define calloc dllcalloc
#define free dllfree
#define malloc dllmalloc
#define realloc dllrealloc
#define _recalloc dll_recalloc
#define _aligned_free dll_aligned_free
#define _aligned_malloc dll_aligned_malloc
#define _aligned_offset_malloc dll_aligned_offset_malloc
#define _aligned_realloc dll_aligned_realloc
#define _aligned_recalloc dll_aligned_recalloc
#define _aligned_offset_realloc dll_aligned_offset_realloc
#define _aligned_offset_recalloc dll_aligned_offset_recalloc

extern FILE *dllstdout;
extern FILE *dllstderr;
extern int dllexitcode;


#endif