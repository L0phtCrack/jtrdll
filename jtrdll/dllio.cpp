#include<stdio.h>
#include<stdarg.h>
#include<signal.h>
#include<string.h>
#include<memory.h>
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

extern "C" int jtrdll_entrypoint(int argc, char **argv);

extern "C"
{

	void *hook_ctx = NULL;
	char appdatadir[_MAX_PATH] = "";
	void(*stdout_hook)(void *ctx, const char *str) = NULL;
	void(*stderr_hook)(void *ctx, const char *str) = NULL;
	int dllexitcode = 0;
	int dllexited = 0;

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
		g_malloc_allocations.erase(_Memory);
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
	}



	JTRDLL_IMPEXP int jtrdll_main(int argc, char **argv, struct JTRDLL_HOOKS *hooks)
	{
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

	JTRDLL_IMPEXP void jtrdll_abort(void)
	{
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

}