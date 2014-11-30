#include<stdio.h>
#include<stdarg.h>
#include<signal.h>
#include<string.h>
#include<memory.h>

extern "C"
{
#include"os.h"
#include"signals.h"
}

#include"jtrdll.h"


extern "C" int jtrdll_entrypoint(int argc, char **argv);

void *hook_ctx=NULL;
void (*stdout_hook)(void *ctx, const char *str)=NULL;
void (*stderr_hook)(void *ctx, const char *str)=NULL;
int dllexitcode=0;
int dllexited=0;

extern "C" 
{

int strprintf(char **psz, const char *szFmt, va_list args)
{
     int nLen;
     if ((nLen = vsnprintf(NULL, 0, szFmt, args)) >= 0)
     {
         char *szRes = (char*) realloc(*psz, nLen + 1);                  
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
	va_start(args,_Format);

	if(stdout_hook)
	{
		char *str=NULL;
		ret=strprintf(&str, _Format, args);
		stdout_hook(hook_ctx,str);
		free(str);
	}
	else
	{
		ret=vprintf(_Format, args);
	}

	va_end(args);
	return ret;
}

int dllfprintf(FILE *f, const char * _Format, ...)
{
	int ret;

	if(stdout_hook && f==stdout)
	{
		va_list args;
		va_start(args,_Format);
		char *str=NULL;
		ret=strprintf(&str, _Format, args);
		va_end(args);

		stdout_hook(hook_ctx,str);
		free(str);
	}
	else if(stderr_hook && f==stderr)
	{
		va_list args;
		va_start(args,_Format);
		char *str=NULL;
		ret=strprintf(&str, _Format, args);
		va_end(args);

		stderr_hook(hook_ctx,str);
		free(str);
	}
	else
	{
		va_list args;
		va_start(args,_Format);
		ret=vfprintf(f, _Format, args);
		va_end(args);
	}

	
	return ret;
}

int dllfputs(const char *str, FILE *f)
{
	if(stdout_hook && f==stdout)
	{
		stdout_hook(hook_ctx, str);
		return (int)strlen(str);
	}
	else if(stderr_hook && f==stderr)
	{
		stderr_hook(hook_ctx, str);
		return (int)strlen(str);
	}

	return fputs(str,f);
}

int dllputs(const char *str)
{
	if(stdout_hook)
	{
		stdout_hook(hook_ctx, str);
		return (int)strlen(str);
	}
	return puts(str);	
}

int dllputchar(int c)
{
	if(stdout_hook)
	{
		char ch[2];
		ch[0]=c;
		ch[1]='\0';
		stdout_hook(hook_ctx, ch);
		return 1;
	}
	return putchar(c);
}

int dllfputc(int c, FILE *f)
{
	if(stdout_hook && f==stdout)
	{
		char ch[2];
		ch[0]=c;
		ch[1]='\0';

		stdout_hook(hook_ctx, ch);
		return 1;
	}
	else if(stderr_hook && f==stderr)
	{
		char ch[2];
		ch[0]=c;
		ch[1]='\0';

		stderr_hook(hook_ctx, ch);
		return 1;
	}
	
	return fputc(c,f);
}

void dllexit(int exitcode)
{
	dllexitcode=exitcode;
	dllexited=1;
	throw (int)1;
}

void dll_exit(int exitcode)
{
	dllexit(exitcode);	
}


JTRDLL_IMPEXP int jtrdll_main(int argc, char **argv, struct JTRDLL_HOOKS *hooks)
{
	try
	{
		if(hooks)
		{
			hook_ctx=hooks->ctx;
			stdout_hook=hooks->stdout_hook;
			stderr_hook=hooks->stderr_hook;
		}
		return jtrdll_entrypoint(argc,argv);
	}
	catch(...)
	{
		return dllexitcode;
	}
}

JTRDLL_IMPEXP void jtrdll_abort(void)
{
	sig_handle_abort(SIGINT);
}

}