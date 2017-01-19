#include<string>
#include<iostream>
#include<vector>
#include<stdio.h>
#ifdef _WIN32
#include<windows.h>
#include"process.h"
#endif
#include"cpuinformation.h"
#include"jtrexe.h"
#include"processhost.h"

/////////////////////////////////////////////////////////////////////////////

HMODULE jtrdll=NULL;
TYPEOF_jtrdll_main *jtrdll_main = NULL;
TYPEOF_jtrdll_abort *jtrdll_abort = NULL;
TYPEOF_jtrdll_get_status *jtrdll_get_status = NULL;
TYPEOF_jtrdll_get_charset_info *jtrdll_get_charset_info = NULL;
TYPEOF_jtrdll_cleanup *jtrdll_cleanup = NULL;
TYPEOF_jtrdll_preflight *jtrdll_preflight = NULL;
TYPEOF_jtrdll_set_extra_opencl_kernel_args *jtrdll_set_extra_opencl_kernel_args = NULL;

/////////////////////////////////////////////////////////////////////////////

bool LoadJTRDLL(std::string jtrdllversion)
{
#ifdef _WIN32
	jtrdll = LoadLibrary((std::string("jtrdll_") + jtrdllversion + ".dll").c_str());
	if (!jtrdll)
	{
		fprintf(stderr, "Missing jtrdll\n");
		return false;
	}

	jtrdll_main = (TYPEOF_jtrdll_main *)GetProcAddress(jtrdll, "jtrdll_main");
	jtrdll_abort = (TYPEOF_jtrdll_abort *)GetProcAddress(jtrdll, "jtrdll_abort");
	jtrdll_get_status = (TYPEOF_jtrdll_get_status *)GetProcAddress(jtrdll, "jtrdll_get_status");
	jtrdll_get_charset_info = (TYPEOF_jtrdll_get_charset_info *)GetProcAddress(jtrdll, "jtrdll_get_charset_info");
	jtrdll_cleanup = (TYPEOF_jtrdll_cleanup *)GetProcAddress(jtrdll, "jtrdll_cleanup");
	jtrdll_preflight = (TYPEOF_jtrdll_preflight *)GetProcAddress(jtrdll, "jtrdll_preflight");
	jtrdll_set_extra_opencl_kernel_args = (TYPEOF_jtrdll_set_extra_opencl_kernel_args *)GetProcAddress(jtrdll, "jtrdll_set_extra_opencl_kernel_args");

	if (!jtrdll_main || !jtrdll_abort || !jtrdll_get_status || !jtrdll_get_charset_info || !jtrdll_cleanup || !jtrdll_preflight || !jtrdll_set_extra_opencl_kernel_args)
	{
		fprintf(stderr, "Malformed jtrdll\n");
		FreeLibrary(jtrdll);
		return false;
	}

#else
#error "to do"
#endif
	return true;
}

void UnloadJTRDLL(void)
{
#ifdef _WIN32
	FreeLibrary(jtrdll);
#else
#error "to do"
#endif
}

/////////////////////////////////////////////////////////////////////////////
#ifdef JTREXE_USE_CRASHDUMP
#include"jtrexe_crashdump.h"
#endif

int main(int argc, char **argv)
{
#ifdef JTREXE_USE_CRASHDUMP
	JTREXE_INSTALL_CRASHDUMP();
#endif

	putenv("CUDA_CACHE_DISABLE=1");

	CPUInformation cpu;
	std::string jtrdllversion, force_jtrdllversion;
	std::vector<char *> args;
	bool processhost = false;
	for (int arg = 0; arg < argc; arg++)
	{
		if (std::string(argv[arg]).compare(0, 8, "--force-") == 0)
		{
			force_jtrdllversion = argv[arg] + 8;
		}
		if (std::string(argv[arg]) == "--processhost")
		{
			processhost = true;
		}
		else
		{
			args.push_back(argv[arg]);
		}
	}
	args.push_back(NULL);

	jtrdllversion = "sse2";
	if (jtrdllversion != force_jtrdllversion && cpu.SSSE3())
	{
		jtrdllversion = "ssse3";
		if (jtrdllversion != force_jtrdllversion && cpu.SSE41())
		{
			jtrdllversion = "sse41";


#ifdef _WIN64
			if (jtrdllversion != force_jtrdllversion && cpu.AVX() && cpu.XSAVE() && cpu.OSXSAVE() && cpu.XMM_SAVED() && cpu.YMM_SAVED())
			{
				jtrdllversion = "avx";
			}

			if (jtrdllversion != force_jtrdllversion && cpu.XOP())
			{
				jtrdllversion = "xop";
			}

			if (jtrdllversion != force_jtrdllversion && cpu.AVX() && cpu.AVX2() && cpu.XSAVE() && cpu.OSXSAVE() && cpu.XMM_SAVED() && cpu.YMM_SAVED() && cpu.MOVBE() && cpu.FMA())
			{
				jtrdllversion = "avx2";
			}
#endif
		}
	}

	if (force_jtrdllversion.length() != 0 && jtrdllversion != force_jtrdllversion)
	{
		fprintf(stderr, "Incompatible instruction set\n");
#ifdef JTREXE_USE_CRASHDUMP
		JTREXE_UNINSTALL_CRASHDUMP();
#endif
		return 1;
	}
	
	if (force_jtrdllversion.length() == 0)
	{
		fprintf(stderr, "Detected %s instruction set\n", jtrdllversion.c_str());
	}
	
	if (!LoadJTRDLL(jtrdllversion))
	{
#ifdef JTREXE_USE_CRASHDUMP
		JTREXE_UNINSTALL_CRASHDUMP();
#endif
		return 1;
	}
	
	int ret;
	if (processhost)
	{
		ret = run_processhost();
	}
	else
	{
		JTRDLL_HOOKS hooks;
#ifdef _WIN32
		memset(&hooks, 0, sizeof(hooks));
		GetTempPath(sizeof(hooks.appdatadir), hooks.appdatadir);
		if (hooks.appdatadir[strlen(hooks.appdatadir) - 1] == '\\')
		{
			hooks.appdatadir[strlen(hooks.appdatadir) - 1] = 0;
		}
		char kernelsdir[MAX_PATH];
		strcpy_s(kernelsdir, sizeof(kernelsdir), hooks.appdatadir);
		strcat_s(kernelsdir, sizeof(kernelsdir), "\\kernels");
		CreateDirectory(kernelsdir, NULL);
#endif
		ret = jtrdll_main((int)args.size() - 1, &(args[0]), &hooks);
	}

	UnloadJTRDLL();

#ifdef JTREXE_USE_CRASHDUMP
	JTREXE_UNINSTALL_CRASHDUMP();
#endif

	return 0;
}