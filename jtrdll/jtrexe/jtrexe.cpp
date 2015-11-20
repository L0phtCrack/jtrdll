#include<string>
#include<vector>
#include<stdio.h>
#ifdef _WIN32
#include<windows.h>
#endif
#include"cpuinformation.h"

typedef int (*TYPEOF_jtrdll_main)(int argc, char **argv, void *hooks);

int main(int argc, char **argv)
{
	CPUInformation cpu;
	std::string jtrdllversion, force_jtrdllversion;
	std::vector<char *> args;
	for (int arg = 0; arg < argc; arg++)
	{
		if (std::string(argv[arg]).compare(0, 8, "--force-") == 0)
		{
			force_jtrdllversion = argv[arg] + 8;
		}
		else
		{
			args.push_back(argv[arg]);
		}
	}
	args.push_back(NULL);

	jtrdllversion  = "sse2";
	if (jtrdllversion!=force_jtrdllversion && cpu.SSSE3())
	{
		jtrdllversion = "ssse3";
		if (jtrdllversion != force_jtrdllversion && cpu.SSE41())
		{
			jtrdllversion = "sse41";
			
			if (jtrdllversion != force_jtrdllversion && cpu.XOP())
			{
				jtrdllversion = "xop";
			}
			else if (jtrdllversion != force_jtrdllversion && cpu.AVX() && cpu.XSAVE() && cpu.OSXSAVE() && cpu.XMM_SAVED() && cpu.YMM_SAVED())
			{
				jtrdllversion = "avx";
			}

			if (jtrdllversion != force_jtrdllversion && cpu.AVX2() && cpu.AVX() && cpu.XSAVE() && cpu.OSXSAVE() && cpu.XMM_SAVED() && cpu.YMM_SAVED())
			{
				jtrdllversion = "avx2";
			}
		}
	}

	if (force_jtrdllversion.length()!=0 && jtrdllversion != force_jtrdllversion)
	{
		fprintf(stderr, "Incompatible instruction set\n");
		exit(1);
	}
	
#ifdef _WIN32
	printf("%s\n", (std::string("jtrdll_") + jtrdllversion + ".dll").c_str());
	HMODULE jtrdll = LoadLibrary((std::string("jtrdll_")+jtrdllversion+".dll").c_str());
	if(!jtrdll)
	{
		fprintf(stderr,"Missing jtrdll\n");
		exit(1);
	}

	TYPEOF_jtrdll_main jtrdll_main = (TYPEOF_jtrdll_main) GetProcAddress(jtrdll,"jtrdll_main");

	if(!jtrdll_main)
	{
		fprintf(stderr,"Malformed jtrdll\n");
		FreeLibrary(jtrdll);
		exit(1);
	}
#else
#error "to do"
#endif

	printf("Detected %s instruction set\n", jtrdllversion.c_str());
	
	int ret = jtrdll_main(args.size()-1,&(args[0]),NULL);
	
#ifdef _WIN32
	FreeLibrary(jtrdll);
#else
#error "to do"
#endif

	return ret;
}