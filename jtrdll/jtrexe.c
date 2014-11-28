#include<stdio.h>

#ifdef _MSC_VER
#define DLLIMPORT __declspec(dllimport) 
#else
#define DLLIMPORT __attribute__((__visibility__("default"))) 
#endif

DLLIMPORT int jtrdll_main(int argc, char **argv, void *hooks);

int main(int argc, char **argv)
{
	return jtrdll_main(argc,argv,NULL);
}