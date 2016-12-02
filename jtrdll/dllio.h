#ifndef __INC_DLLIO_H
#define __INC_DLLIO_H

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wformat"
#endif

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/stat.h>
#include<io.h>
#ifdef _WIN32
#include<wchar.h>
#include<tchar.h>
#endif
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
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueReadBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueReadBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, const size_t * buffer_origin, const size_t * host_origin, const size_t * region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_1;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueWriteBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset, size_t cb, const void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueWriteBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, const size_t * buffer_origin, const size_t * host_origin, const size_t * region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, const void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_1;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyBuffer(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, size_t src_offset, size_t dst_offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyBufferRect(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, const size_t * src_origin, const size_t * dst_origin, const size_t * region, size_t src_row_pitch, size_t src_slice_pitch, size_t dst_row_pitch, size_t dst_slice_pitch, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_1;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueReadImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_read, const size_t * origin[], const size_t * region[], size_t row_pitch, size_t slice_pitch, void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueWriteImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_write, const size_t * origin[], const size_t * region[], size_t input_row_pitch, size_t input_slice_pitch, const void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyImage(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_image, const size_t * src_origin[], const size_t * dst_origin[], const size_t * region[], cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyImageToBuffer(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_buffer, const size_t * src_origin[], const size_t * region[], size_t dst_offset, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyBufferToImage(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_image, size_t src_offset, const size_t * dst_origin[], const size_t * region[], cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY void * CL_API_CALL dllclEnqueueMapBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_map, cl_map_flags map_flags, size_t offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event, cl_int * errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY void * CL_API_CALL dllclEnqueueMapImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_map, cl_map_flags map_flags, const size_t * origin[], const size_t * region[], size_t * image_row_pitch, size_t * image_slice_pitch, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event, cl_int * errcode_ret) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueUnmapMemObject(cl_command_queue command_queue, cl_mem memobj, void * mapped_ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueNDRangeKernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t * global_work_offset, const size_t * global_work_size, const size_t * local_work_size, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueTask(cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueNativeKernel(cl_command_queue command_queue, void (CL_CALLBACK *user_func)(void *), void * args, size_t cb_args, cl_uint num_mem_objects, const cl_mem * mem_list, const void ** args_mem_loc, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueMarker(cl_command_queue command_queue, cl_event * event) CL_API_SUFFIX__VERSION_1_0;
//CL_API_ENTRY cl_int CL_API_CALL clEnqueueWaitForEvents(cl_command_queue command_queue, cl_uint num_events, const cl_event * event_list) CL_API_SUFFIX__VERSION_1_0;
//CL_API_ENTRY cl_int CL_API_CALL clEnqueueBarrier(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0;
CL_API_ENTRY cl_int CL_API_CALL dllclReleaseEvent(cl_event event) CL_API_SUFFIX__VERSION_1_0;

////////////////////////////////////////////////////////////////////////

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
#define clEnqueueReadBuffer dllclEnqueueReadBuffer
#define clEnqueueReadBufferRect dllclEnqueueReadBufferRect
#define clEnqueueWriteBuffer dllclEnqueueWriteBuffer
#define clEnqueueWriteBufferRect dllclEnqueueWriteBufferRect
#define clEnqueueCopyBuffer dllclEnqueueCopyBuffer
#define clEnqueueCopyBufferRect dllclEnqueueCopyBufferRect
#define clEnqueueReadImage dllclEnqueueReadImage
#define clEnqueueWriteImage dllclEnqueueWriteImage
#define clEnqueueCopyImage dllclEnqueueCopyImage
#define clEnqueueCopyImageToBuffer dllclEnqueueCopyImageToBuffer
#define clEnqueueCopyBufferToImage dllclEnqueueCopyBufferToImage
#define clEnqueueMapBuffer dllclEnqueueMapBuffer
#define clEnqueueMapImage dllclEnqueueMapImage
#define clEnqueueUnmapMemObject dllclEnqueueUnmapMemObject
#define clEnqueueNDRangeKernel dllclEnqueueNDRangeKernel
#define clEnqueueTask dllclEnqueueTask
#define clEnqueueNativeKernel dllclEnqueueNativeKernel
#define clEnqueueMarker dllclEnqueueMarker
#define clReleaseEvent dllclReleaseEvent

extern FILE *dllstdout;
extern FILE *dllstderr;
extern int dllexitcode;

/* utf 8 functions */
int utf8_stat(const char * _Filename, struct stat * _Stat);
FILE * utf8_fopen(const char * _Filename, const char * _Mode);
int utf8_chmod(const char * _Filename, int _AccessMode);
int utf8__open(const char * _Filename, int _OpenFlag, int _PMode);
int utf8__unlink(const char * _Filename);

struct utf8_stat {
	_dev_t     st_dev;
	_ino_t     st_ino;
	unsigned short st_mode;
	short      st_nlink;
	short      st_uid;
	short      st_gid;
	_dev_t     st_rdev;
	_off_t     st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
};

#define fopen utf8_fopen
#define stat utf8_stat
#define _open utf8__open
#define _unlink utf8__unlink
#define chmod utf8_chmod

#endif