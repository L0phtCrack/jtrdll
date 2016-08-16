#ifndef __INC_JTREXE_H
#define __INC_JTREXE_H

#include "jtrdll.h"

#ifdef _WIN32
extern HMODULE jtrdll;
#else
void *jtrdll;
#endif

extern TYPEOF_jtrdll_main *jtrdll_main;
extern TYPEOF_jtrdll_abort *jtrdll_abort;
extern TYPEOF_jtrdll_get_status *jtrdll_get_status;
extern TYPEOF_jtrdll_get_charset_info *jtrdll_get_charset_info;
extern TYPEOF_jtrdll_cleanup *jtrdll_cleanup;
extern TYPEOF_jtrdll_preflight *jtrdll_preflight;
extern TYPEOF_jtrdll_set_extra_opencl_kernel_args *jtrdll_set_extra_opencl_kernel_args;
	

#endif