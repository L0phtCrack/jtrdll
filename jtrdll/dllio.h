#ifndef __INC_DLLIO_H
#define __INC_DLLIO_H

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wformat"
#endif

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenCL/cl_ext.h>
#else
#include <CL/cl.h>
#include <CL/cl_ext.h>
#endif

// stdio.h
int dllprintf(const char * _Format, ...);
int dllfprintf(FILE *f, const char * _Format, ...);
int dllfputs(const char *str, FILE *f);
int dllputs(const char *str);
int dllputchar(int c);
int dllfputc(int c, FILE *f);

// malloc.h/stdlib.h
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

// cl.h
CL_API_ENTRY cl_mem CL_API_CALL dllclCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclReleaseMemObject(cl_mem memobj) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_context CL_API_CALL dllclCreateContext(const cl_context_properties * properties, cl_uint num_devices, const cl_device_id *devices, void (CL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *), void *user_data, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclReleaseContext(cl_context context) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_command_queue CL_API_CALL dllclCreateCommandQueue(cl_context context, cl_device_id device, cl_command_queue_properties properties, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclReleaseCommandQueue(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_program CL_API_CALL dllclCreateProgramWithSource(cl_context context, cl_uint count, const char **strings, const size_t *lengths, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_program CL_API_CALL dllclCreateProgramWithBinary(cl_context context, cl_uint num_devices, const cl_device_id *device_list, const size_t *lengths, const unsigned char **binaries, cl_int *binary_status, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclReleaseProgram(cl_program program) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_kernel CL_API_CALL dllclCreateKernel(cl_program program, const char *kernel_name, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclReleaseKernel(cl_kernel kernel) CL_API_SUFFIX__VERSION_1_0;

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
#define clCreateBuffer dllclCreateBuffer
#define clReleaseMemObject dllclReleaseMemObject
#define clCreateContext dllclCreateContext
#define clReleaseContext dllclReleaseContext
#define clCreateCommandQueue dllclCreateCommandQueue
#define clReleaseCommandQueue dllclReleaseCommandQueue
#define clCreateProgramWithSource dllclCreateProgramWithSource
#define clCreateProgramWithBinary dllclCreateProgramWithBinary
#define clReleaseProgram dllclReleaseProgram
#define clCreateKernel dllclCreateKernel
#define clReleaseKernel dllclReleaseKernel

extern FILE *dllstdout;
extern FILE *dllstderr;
extern int dllexitcode;


#endif