#include<string>
#include<stdio.h>
#ifdef _WIN32
#include<windows.h>
#endif
#include"cpuinformation.h"

typedef int (*TYPEOF_jtrdll_main)(int argc, char **argv, void *hooks);

int main(int argc, char **argv)
{
	CPUInformation cpu;
	std::string jtrdllversion;
	
	jtrdllversion  = "sse2";
	if (cpu.SSSE3())
	{
		jtrdllversion = "ssse3";
		if (cpu.SSE41())
		{
			jtrdllversion = "sse41";
			
			if (cpu.XOP())
			{
				jtrdllversion = "xop";
			}
			else if (cpu.AVX())
			{
				jtrdllversion = "avx";
			}
			
			if (cpu.AVX2())
			{
				jtrdllversion = "avx2";
			}
		}
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
	
	int ret = jtrdll_main(argc,argv,NULL);
	
#ifdef _WIN32
	FreeLibrary(jtrdll);
#else
#error "to do"
#endif

	return ret;
}