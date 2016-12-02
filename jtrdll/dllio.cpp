#ifdef _WIN32
#include<Windows.h>
#include<delayimp.h>
#undef MEM_FREE
#endif

#include<stdio.h>
#include<stdarg.h>
#include<signal.h>
#include<string.h>
#include<memory.h>
#include<malloc.h>
#include<sys/stat.h>
#include<io.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <OpenCL/cl_ext.h>
#else
#include <CL/cl.h>
#include <CL/cl_ext.h>
#endif
#include<exception>
#include<hash_set>



extern "C"
{

#define private _private // hack to work around poorly chosen keyword

#include"os.h"
#include"signals.h"
#include"charset.h"

#undef private

}

#include"jtrdll.h"

// memory allocation tables
std::hash_set<void *> g_malloc_allocations;
std::hash_set<void *> g__aligned_malloc_allocations;
//std::hash_set<void *> g__malloca_allocations;
std::hash_set<cl_event> g_cl_events;
std::hash_set<cl_mem> g_cl_mem_objects;
std::hash_set<cl_context> g_cl_contexts;
std::hash_set<cl_command_queue> g_cl_command_queues;
std::hash_set<cl_program> g_cl_programs; 
std::hash_set<cl_kernel> g_cl_kernels;


///////////////////////////////////////////////////////////////////////////////////////////

extern "C" int jtrdll_entrypoint(int argc, char **argv);

extern "C"
{

	void *hook_ctx = NULL;
	char appdatadir[_MAX_PATH] = "";
	void(*stdout_hook)(void *ctx, const char *str) = NULL;
	void(*stderr_hook)(void *ctx, const char *str) = NULL;
	int dllexitcode = 0;
	int dllexited = 0;
	char extra_opencl_kernel_args[1024]="";

	int strprintf(char **psz, const char *szFmt, va_list args)
	{
		int nLen;
		if ((nLen = vsnprintf(NULL, 0, szFmt, args)) >= 0)
		{
			char *szRes = (char*)realloc(*psz, nLen + 1);
			if (szRes)
				vsnprintf(*psz = szRes, nLen + 1, szFmt, args);
			else
				nLen = -1;
		}
		return nLen;
	}


	int dllprintf(const char * _Format, ...)
	{
		int ret;

		va_list args;
		va_start(args, _Format);

		if (stdout_hook)
		{
			char *str = NULL;
			ret = strprintf(&str, _Format, args);
			stdout_hook(hook_ctx, str);
			free(str);
		}
		else
		{
			ret = vprintf(_Format, args);
		}

		va_end(args);
		return ret;
	}

	int dllfprintf(FILE *f, const char * _Format, ...)
	{
		int ret;

		if (stdout_hook && f == stdout)
		{
			va_list args;
			va_start(args, _Format);
			char *str = NULL;
			ret = strprintf(&str, _Format, args);
			va_end(args);

			stdout_hook(hook_ctx, str);
			free(str);
		}
		else if (stderr_hook && f == stderr)
		{
			va_list args;
			va_start(args, _Format);
			char *str = NULL;
			ret = strprintf(&str, _Format, args);
			va_end(args);

			stderr_hook(hook_ctx, str);
			free(str);
		}
		else
		{
			va_list args;
			va_start(args, _Format);
			ret = vfprintf(f, _Format, args);
			va_end(args);
		}


		return ret;
	}

	int dllfputs(const char *str, FILE *f)
	{
		if (stdout_hook && f == stdout)
		{
			stdout_hook(hook_ctx, str);
			return (int)strlen(str);
		}
		else if (stderr_hook && f == stderr)
		{
			stderr_hook(hook_ctx, str);
			return (int)strlen(str);
		}

		return fputs(str, f);
	}

	int dllputs(const char *str)
	{
		if (stdout_hook)
		{
			stdout_hook(hook_ctx, str);
			return (int)strlen(str);
		}
		return puts(str);
	}

	int dllputchar(int c)
	{
		if (stdout_hook)
		{
			char ch[2];
			ch[0] = c;
			ch[1] = '\0';
			stdout_hook(hook_ctx, ch);
			return 1;
		}
		return putchar(c);
	}

	int dllfputc(int c, FILE *f)
	{
		if (stdout_hook && f == stdout)
		{
			char ch[2];
			ch[0] = c;
			ch[1] = '\0';

			stdout_hook(hook_ctx, ch);
			return 1;
		}
		else if (stderr_hook && f == stderr)
		{
			char ch[2];
			ch[0] = c;
			ch[1] = '\0';

			stderr_hook(hook_ctx, ch);
			return 1;
		}

		return fputc(c, f);
	}

	void dllexit(int exitcode)
	{
		dllexitcode = exitcode;
		dllexited = 1;
		throw (int)1;
	}

	void dll_exit(int exitcode)
	{
		dllexit(exitcode);
	}

	/////////////////////////////////////////

	void * dllcalloc(size_t _Count, size_t _Size)
	{
		void *ret = calloc(_Count, _Size);
		if (ret)
		{
			g_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void   dllfree(void * _Memory)
	{
		free(_Memory);
		g_malloc_allocations.erase(_Memory);
	}

	void * dllmalloc(size_t _Size)
	{
		void *ret = malloc(_Size);
		if (ret)
		{
			g_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void * dllrealloc(void * _Memory, size_t _NewSize)
	{
		void *ret = realloc(_Memory, _NewSize);
		if (ret)
		{
			g_malloc_allocations.erase(_Memory);
			g_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void * dll_recalloc(void * _Memory, size_t _Count, size_t _Size)
	{
		void *ret = _recalloc(_Memory, _Count, _Size);
		if (ret)
		{
			g_malloc_allocations.erase(_Memory);
			g_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void dll_aligned_free(void * _Memory)
	{
		_aligned_free(_Memory);
		g__aligned_malloc_allocations.erase(_Memory);
	}

	void * dll_aligned_malloc(size_t _Size, size_t _Alignment)
	{
		void *ret = _aligned_malloc(_Size, _Alignment);
		if (ret)
		{
			g__aligned_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void * dll_aligned_offset_malloc(size_t _Size, size_t _Alignment, size_t _Offset)
	{
		void *ret = _aligned_offset_malloc(_Size, _Alignment, _Offset);
		if (ret)
		{
			g__aligned_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void * dll_aligned_realloc(void * _Memory, size_t _NewSize, size_t _Alignment)
	{
		void *ret = _aligned_realloc(_Memory, _NewSize, _Alignment);
		if (ret)
		{
			g__aligned_malloc_allocations.erase(_Memory);
			g__aligned_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void * dll_aligned_recalloc(void * _Memory, size_t _Count, size_t _Size, size_t _Alignment)
	{
		void *ret = _aligned_recalloc(_Memory, _Count, _Size, _Alignment);
		if (ret)
		{
			g__aligned_malloc_allocations.erase(_Memory);
			g__aligned_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void * dll_aligned_offset_realloc(void * _Memory, size_t _NewSize, size_t _Alignment, size_t _Offset)
	{
		void *ret = _aligned_offset_realloc(_Memory, _NewSize, _Alignment, _Offset);
		if (ret)
		{
			g__aligned_malloc_allocations.erase(_Memory);
			g__aligned_malloc_allocations.insert(ret);
		}
		return ret;
	}

	void * dll_aligned_offset_recalloc(void * _Memory, size_t _Count, size_t _Size, size_t _Alignment, size_t _Offset)
	{
		void *ret = _aligned_offset_recalloc(_Memory, _Count, _Size, _Alignment, _Offset);
		if (ret)
		{
			g__aligned_malloc_allocations.erase(_Memory);
			g__aligned_malloc_allocations.insert(ret);
		}
		return ret;
	}

	/////////////////////////////////////////

	CL_API_ENTRY cl_mem CL_API_CALL dllclCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		cl_mem ret = clCreateBuffer(context, flags, size, host_ptr, errcode_ret);
		if (ret)
		{
			g_cl_mem_objects.insert(ret);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclReleaseMemObject(cl_mem memobj) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clReleaseMemObject(memobj);
		if (ret == CL_SUCCESS)
		{
			g_cl_mem_objects.erase(memobj);
		}
		return ret;
	}

	CL_API_ENTRY cl_context CL_API_CALL dllclCreateContext(const cl_context_properties * properties, cl_uint num_devices, const cl_device_id *devices, void (CL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *), void *user_data, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		cl_context ret = clCreateContext(properties, num_devices, devices, pfn_notify, user_data, errcode_ret);
		if (ret)
		{
			g_cl_contexts.insert(ret);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclReleaseContext(cl_context context) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clReleaseContext(context);
		if (ret == CL_SUCCESS)
		{
			g_cl_contexts.erase(context);
		}
		return ret;
	}

	CL_API_ENTRY cl_command_queue CL_API_CALL dllclCreateCommandQueue(cl_context context, cl_device_id device, cl_command_queue_properties properties, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		cl_command_queue ret = clCreateCommandQueue(context, device, properties, errcode_ret);
		if (ret)
		{
			g_cl_command_queues.insert(ret);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclReleaseCommandQueue(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clReleaseCommandQueue(command_queue);
		if (ret == CL_SUCCESS)
		{
			g_cl_command_queues.erase(command_queue);
		}
		return ret;
	}

	CL_API_ENTRY cl_program CL_API_CALL dllclCreateProgramWithSource(cl_context context, cl_uint count, const char **strings, const size_t *lengths, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		cl_program ret = clCreateProgramWithSource(context, count, strings, lengths, errcode_ret);
		if (ret)
		{
			g_cl_programs.insert(ret);
		}

		return ret;
	}

	CL_API_ENTRY cl_program CL_API_CALL dllclCreateProgramWithBinary(cl_context context, cl_uint num_devices, const cl_device_id *device_list, const size_t *lengths, const unsigned char **binaries, cl_int *binary_status, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		cl_program ret = clCreateProgramWithBinary(context, num_devices, device_list, lengths, binaries, binary_status, errcode_ret);
		if (ret)
		{
			g_cl_programs.insert(ret);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclReleaseProgram(cl_program program) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clReleaseProgram(program);
		if (ret == CL_SUCCESS)
		{
			g_cl_programs.erase(program);
		}
		return ret;
	}

	CL_API_ENTRY cl_kernel CL_API_CALL dllclCreateKernel(cl_program program, const char *kernel_name, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		cl_kernel ret = clCreateKernel(program, kernel_name, errcode_ret);
		if (ret)
		{
			g_cl_kernels.insert(ret);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclReleaseKernel(cl_kernel kernel) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clReleaseKernel(kernel);
		if (ret == CL_SUCCESS)
		{
			g_cl_kernels.erase(kernel);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueReadBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueReadBuffer(command_queue, buffer, blocking_read, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueReadBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, const size_t * buffer_origin, const size_t * host_origin, const size_t * region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_1
	{
		cl_int ret = clEnqueueReadBufferRect(command_queue, buffer, blocking_read, buffer_origin, host_origin, region, buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueWriteBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset, size_t cb, const void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueWriteBuffer(command_queue, buffer, blocking_write, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueWriteBufferRect(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, const size_t * buffer_origin, const size_t * host_origin, const size_t * region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, const void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_1
	{
		cl_int ret = clEnqueueWriteBufferRect(command_queue, buffer, blocking_write, buffer_origin, host_origin, region, buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyBuffer(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, size_t src_offset, size_t dst_offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueCopyBuffer(command_queue, src_buffer, dst_buffer, src_offset, dst_offset, cb, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyBufferRect(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer, const size_t * src_origin, const size_t * dst_origin, const size_t * region, size_t src_row_pitch, size_t src_slice_pitch, size_t dst_row_pitch, size_t dst_slice_pitch, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_1
	{
		cl_int ret = clEnqueueCopyBufferRect(command_queue, src_buffer, dst_buffer, src_origin, dst_origin, region, src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueReadImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_read, const size_t * origin, const size_t * region, size_t row_pitch, size_t slice_pitch, void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueReadImage(command_queue, image, blocking_read, origin, region, row_pitch, slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueWriteImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_write, const size_t * origin, const size_t * region, size_t input_row_pitch, size_t input_slice_pitch, const void * ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueWriteImage(command_queue, image, blocking_write, origin, region, input_row_pitch, input_slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyImage(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_image, const size_t * src_origin, const size_t * dst_origin, const size_t * region, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueCopyImage(command_queue, src_image, dst_image, src_origin, dst_origin, region, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyImageToBuffer(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_buffer, const size_t * src_origin, const size_t * region, size_t dst_offset, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueCopyImageToBuffer(command_queue, src_image, dst_buffer, src_origin, region, dst_offset, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueCopyBufferToImage(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_image, size_t src_offset, const size_t * dst_origin, const size_t * region, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueCopyBufferToImage(command_queue, src_buffer, dst_image, src_offset, dst_origin, region, num_events_in_wait_list, event_wait_list, event);
		if (ret == CL_SUCCESS && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY void * CL_API_CALL dllclEnqueueMapBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_map, cl_map_flags map_flags, size_t offset, size_t cb, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event, cl_int * errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		void *ret = clEnqueueMapBuffer(command_queue, buffer, blocking_map, map_flags, offset, cb, num_events_in_wait_list, event_wait_list, event, errcode_ret);
		if (ret && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY void * CL_API_CALL dllclEnqueueMapImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_map, cl_map_flags map_flags, const size_t * origin, const size_t * region, size_t * image_row_pitch, size_t * image_slice_pitch, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event, cl_int * errcode_ret) CL_API_SUFFIX__VERSION_1_0
	{
		void *ret = clEnqueueMapImage(command_queue, image, blocking_map, map_flags, origin, region, image_row_pitch, image_slice_pitch, num_events_in_wait_list, event_wait_list, event, errcode_ret);
		if (ret && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueUnmapMemObject(cl_command_queue command_queue, cl_mem memobj, void * mapped_ptr, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueUnmapMemObject(command_queue, memobj, mapped_ptr, num_events_in_wait_list, event_wait_list, event);
		if (ret && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueNDRangeKernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t * global_work_offset, const size_t * global_work_size, const size_t * local_work_size, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
		if (ret && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueTask(cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueTask(command_queue, kernel, num_events_in_wait_list, event_wait_list, event);
		if (ret && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueNativeKernel(cl_command_queue command_queue, void (CL_CALLBACK *user_func)(void *), void * args, size_t cb_args, cl_uint num_mem_objects, const cl_mem * mem_list, const void ** args_mem_loc, cl_uint num_events_in_wait_list, const cl_event * event_wait_list, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueNativeKernel(command_queue, user_func, args, cb_args, num_mem_objects, mem_list, args_mem_loc, num_events_in_wait_list, event_wait_list, event);
		if (ret && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclEnqueueMarker(cl_command_queue command_queue, cl_event * event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clEnqueueMarker(command_queue, event);
		if (ret && event != NULL)
		{
			g_cl_events.insert(*event);
		}
		return ret;
	}

	CL_API_ENTRY cl_int CL_API_CALL dllclReleaseEvent(cl_event event) CL_API_SUFFIX__VERSION_1_0
	{
		cl_int ret = clReleaseEvent(event);
		if (ret == CL_SUCCESS)
		{
			g_cl_events.erase(event);
		}
		return ret;
	}

	/////////////////////////////////////////

	class __sigill :public std::exception
	{

	};

	void sigill_handler(int signal)
	{
		throw __sigill();
	}

	typedef void(*SIGPTR)(int);



	JTRDLL_IMPEXP void jtrdll_cleanup(void)
	{
		for (std::hash_set<void *>::iterator iter = g_malloc_allocations.begin(); iter != g_malloc_allocations.end(); iter++)
		{
			void *ptr = *iter;
			free(ptr);
		}
		g_malloc_allocations.clear();

		for (std::hash_set<void *>::iterator iter = g__aligned_malloc_allocations.begin(); iter != g__aligned_malloc_allocations.end(); iter++)
		{
			void *ptr = *iter;
			_aligned_free(ptr);
		}
		g__aligned_malloc_allocations.clear();

		//	for (std::hash_set<void *>::iterator iter = g__malloca_allocations.begin(); iter != g__malloca_allocations.end(); iter++)
		//	{
		//		void *ptr = *iter;
		//		_freea(ptr);
		//	}
		//	g__malloca_allocations.clear();

		for (std::hash_set<cl_event>::iterator iter = g_cl_events.begin(); iter != g_cl_events.end(); iter++)
		{
			cl_event event = *iter;
			clReleaseEvent(event);
		}
		g_cl_events.clear();

		for (std::hash_set<cl_mem>::iterator iter = g_cl_mem_objects.begin(); iter != g_cl_mem_objects.end(); iter++)
		{
			cl_mem mem = *iter;
			clReleaseMemObject(mem);
		}
		g_cl_mem_objects.clear();

		for (std::hash_set<cl_kernel>::iterator iter = g_cl_kernels.begin(); iter != g_cl_kernels.end(); iter++)
		{
			cl_kernel kernel = *iter;
			clReleaseKernel(kernel);
		}
		g_cl_kernels.clear();

		for (std::hash_set<cl_program>::iterator iter = g_cl_programs.begin(); iter != g_cl_programs.end(); iter++)
		{
			cl_program program = *iter;
			clReleaseProgram(program);
		}
		g_cl_programs.clear();

		for (std::hash_set<cl_command_queue>::iterator iter = g_cl_command_queues.begin(); iter != g_cl_command_queues.begin(); iter++)
		{
			cl_command_queue command_queue = *iter;
			clReleaseCommandQueue(command_queue);
		}
		g_cl_command_queues.clear();

		for (std::hash_set<cl_context>::iterator iter = g_cl_contexts.begin(); iter != g_cl_contexts.end(); iter++)
		{
			cl_context context = *iter;
			clReleaseContext(context);
		}
		g_cl_contexts.clear();

#ifdef WIN32
		if (GetModuleHandleA("OpenCL.dll")!=NULL)
		{
			clUnloadCompiler();
			BOOL ret = __FUnloadDelayLoadedDLL2("OpenCL.dll");
			if (!ret)
			{
				//fprintf(stderr,"unable to unload OpenCL.dll\n");
			}
		}
#endif
	}
	 


	JTRDLL_IMPEXP int jtrdll_main(int argc, char **argv, struct JTRDLL_HOOKS *hooks)
	{

//		int tmp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
//		tmp |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_EVERY_1024_DF;//|_CRTDBG_CHECK_ALWAYS_DF; //|_CRTDBG_LEAK_CHECK_DF;// | _CRTDBG_CHECK_EVERY_16_DF;
//		_CrtSetDbgFlag(tmp);


		SIGPTR oldhandler = signal(SIGILL, sigill_handler);

		jtrdll_cleanup();

		try
		{
			if (hooks)
			{
				hook_ctx = hooks->ctx;

				strncpy(appdatadir, hooks->appdatadir, _MAX_PATH);
				appdatadir[_MAX_PATH - 1] = 0;

				stdout_hook = hooks->stdout_hook;
				stderr_hook = hooks->stderr_hook;
			}

			int ret = jtrdll_entrypoint(argc, argv);

			signal(SIGILL, oldhandler);
			return ret;
		}
		catch (__sigill __sig)
		{
			if (hooks)
			{
				hooks->caught_sigill = 1;
			}

			signal(SIGILL, oldhandler);
			return -1;
		}
		catch (...)
		{

			signal(SIGILL, oldhandler);
			return dllexitcode;
		}
	}

	JTRDLL_IMPEXP void jtrdll_abort(int timeout)
	{
		if (timeout)
		{
			aborted_by_timer = 1;
		}
		sig_handle_abort(SIGINT);
	}

	JTRDLL_IMPEXP int jtrdll_get_charset_info(const char *path, unsigned char * min, unsigned char *max, unsigned char *len, unsigned char *count, unsigned char allchars[256])
	{
		struct charset_header header;
		FILE *csf = fopen(path, "rb");
		if (!csf)
		{
			return -1;
		}
		if (charset_read_header(csf, &header) == -1)
		{
			fclose(csf);
			return -1;
		}

		*min = header.min;
		*max = header.max;
		*len = header.length;
		*count = header.count;

		if (fread(allchars, header.count, 1, csf) != 1)
		{
			fclose(csf);
			return -1;
		}

		fclose(csf);

		return 0;
	}


	int jtrdll_is_preflight = 0;
	unsigned long jtrdll_preflight_salt_count;
	unsigned long jtrdll_preflight_wordlist_rule_count;
	//unsigned long long jtrdll_preflight_wordlist_candidates;
	unsigned long long jtrdll_preflight_mask_candidates;
	unsigned long long jtrdll_preflight_incremental_candidates;

	void null_stdhook(void *, const char *)
	{
	}

	JTRDLL_IMPEXP void jtrdll_set_extra_opencl_kernel_args(const char *args)
	{
		strncpy(extra_opencl_kernel_args, args, 1024);
		extra_opencl_kernel_args[1023] = 0;
	}

	JTRDLL_IMPEXP void jtrdll_preflight(int argc, char **argv, struct JTRDLL_HOOKS *hooks, struct JTRDLL_PREFLIGHT *jtrdllpreflight)
	{
	
		// set preflight bit
		jtrdll_is_preflight = 1;
		memset(jtrdllpreflight, 0, sizeof(JTRDLL_PREFLIGHT));
		jtrdll_preflight_salt_count = 0;
		jtrdll_preflight_wordlist_rule_count = 0;
		//jtrdll_preflight_wordlist_candidates = 0;
		jtrdll_preflight_mask_candidates = 0;
		jtrdll_preflight_incremental_candidates = 0;

		// run with preflighting
		int ret = jtrdll_main(argc, argv, hooks);

		// reset preflight
		jtrdll_is_preflight = 0;

		if (ret == 0)
		{
			jtrdllpreflight->valid = 1;
			jtrdllpreflight->incremental_candidates = jtrdll_preflight_incremental_candidates;
			jtrdllpreflight->mask_candidates = jtrdll_preflight_mask_candidates;
			jtrdllpreflight->salt_count = jtrdll_preflight_salt_count;
			jtrdllpreflight->wordlist_rule_count = jtrdll_preflight_wordlist_rule_count;
		}
	}


	// utf8 libc functions
	int utf8_stat(const char * _Filename, struct stat * _Stat)
	{
#if defined(JTRDLL) && defined(_WIN32)
		int reqlen = MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), NULL, 0);
		wchar_t *wchfname = (wchar_t *)malloc((reqlen+1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), wchfname, reqlen);
		wchfname[reqlen] = 0;

#ifndef _USE_32BIT_TIME_T
		int ret = _wstat(wchfname, (struct _stat64i32 *)_Stat);
#else
		int ret = _wstat(wchfname, (struct _stat32 *)_Stat);
#endif
		free(wchfname);

		if (ret==0)
		{
			return ret;
		}
		return stat(_Filename, _Stat);
#else
		return stat(_Filename, _Stat);
#endif
	}
	
	FILE * utf8_fopen(const char * _Filename, const char * _Mode)
	{
#if defined(JTRDLL) && defined(_WIN32)
		
		int reqlen = MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), NULL, 0);
		wchar_t *wchfname = (wchar_t *)malloc((reqlen+1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), wchfname, reqlen);
		wchfname[reqlen] = 0;

		reqlen = MultiByteToWideChar(CP_UTF8, 0, _Mode, (int)strlen(_Mode), NULL, 0);
		wchar_t *wchmode = (wchar_t *)malloc((reqlen+1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, _Mode, (int)strlen(_Mode), wchmode, reqlen);
		wchmode[reqlen] = 0;
		/*
		for (wchar_t *ch = wchfname; *ch; ch++)
		{
			if (*ch == L'/')
				*ch = L'\\';
		}
		*/
		FILE *ret = _wfopen(wchfname, wchmode);

		free(wchfname);
		free(wchmode);

		if(ret)
			return ret;

		return fopen(_Filename, _Mode);
#else
		return fopen(_Filename, _Mode);
#endif
	}

	int utf8_chmod(const char * _Filename, int _AccessMode)
	{
#if defined(JTRDLL) && defined(_WIN32)
		int reqlen = MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), NULL, 0);
		wchar_t *wchfname = (wchar_t *)malloc((reqlen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), wchfname, reqlen);
		wchfname[reqlen] = 0;

		int ret = _wchmod(wchfname, _AccessMode);
		free(wchfname);

		if (ret != -1)
		{
			return ret;
		}
		return chmod(_Filename, _AccessMode);
#else
		return chmod(_Filename, _AccessMode);
#endif
	}

	int utf8__open(const char * _Filename, int _OpenFlag, int _PMode)
	{
#if defined(JTRDLL) && defined(_WIN32)
		int reqlen = MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), NULL, 0);
		wchar_t *wchfname = (wchar_t *)malloc((reqlen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), wchfname, reqlen);
		wchfname[reqlen] = 0;

		int ret = _wopen(wchfname, _OpenFlag, _PMode);
		free(wchfname);

		if (ret != -1)
		{
			return ret;
		}
		return _open(_Filename, _OpenFlag, _PMode);
#else
		return _open(_Filename, _OpenFlag, _PMode);
#endif
	}

	int utf8__unlink(const char * _Filename)
	{
#if defined(JTRDLL) && defined(_WIN32)
		int reqlen = MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), NULL, 0);
		wchar_t *wchfname = (wchar_t *)malloc((reqlen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, _Filename, (int)strlen(_Filename), wchfname, reqlen);
		wchfname[reqlen] = 0;

		SetFileAttributesW(wchfname, GetFileAttributesW(wchfname) & ~FILE_ATTRIBUTE_READONLY);
		
		int ret = _wunlink(wchfname);
		free(wchfname);

		if (ret != -1)
		{
			return ret;
		}
 		return _unlink(_Filename);
#else
		return _unlink(_Filename);
#endif
	}



}