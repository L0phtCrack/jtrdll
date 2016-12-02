#ifndef __INC_JTRDLL_H
#define __INC_JTRDLL_H

#include<stdlib.h>

#ifdef JTRDLL_EXPORTS
#ifdef _MSC_VER
#define JTRDLL_IMPEXP __declspec(dllexport) 
#else
#define JTRDLL_IMPEXP __attribute__((__visibility__("default"))) 
#endif
#else
#ifdef _MSC_VER
#define JTRDLL_IMPEXP __declspec(dllimport) 
#else
#define JTRDLL_IMPEXP __attribute__((__visibility__("default"))) 
#endif
#endif

struct JTRDLL_PREFLIGHT
{
	int valid;
	unsigned long salt_count;
	unsigned long wordlist_rule_count;
	//unsigned long long wordlist_candidates;
	unsigned long long mask_candidates;
	unsigned long long incremental_candidates;
};

struct JTRDLL_STATUS
{
	int stage;
	double percent;
	unsigned int time;
	unsigned int eta;
	unsigned long long guess_count;
	unsigned long long candidates;
	unsigned long long guesses_per_second;
	unsigned long long candidates_per_second;
	unsigned long long crypts_per_second;
	unsigned long long combinations_per_second;
	int temperature;
	int fanspeed;
	int utilization;
	char word1[0x80];
	char word2[0x80];
};

struct JTRDLL_HOOKS 
{
	char appdatadir[_MAX_PATH];
	int caught_sigill;
	void *ctx;
	void (*stdout_hook)(void *ctx, const char *str);
	void (*stderr_hook)(void *ctx, const char *str);
};

#ifdef __cplusplus
extern "C" {
#endif

	
#ifdef JTRDLL_IMPORTS
JTRDLL_IMPEXP void jtrdll_get_status(struct JTRDLL_STATUS *status);
JTRDLL_IMPEXP int jtrdll_main(int argc, char **argv, struct JTRDLL_HOOKS *hooks);
JTRDLL_IMPEXP void jtrdll_abort(int timeout);
JTRDLL_IMPEXP int jtrdll_get_charset_info(const char *path, unsigned char * charmin, unsigned char *charmax, unsigned char *len, unsigned char *count, unsigned char allchars[256]);
JTRDLL_IMPEXP void jtrdll_cleanup(void);
JTRDLL_IMPEXP void jtrdll_preflight(int argc, char **argv, struct JTRDLL_HOOKS *hooks, struct JTRDLL_PREFLIGHT *jtrdllpreflight);
#endif

typedef int (TYPEOF_jtrdll_main)(int argc, char **argv, struct JTRDLL_HOOKS *hooks);
typedef void (TYPEOF_jtrdll_abort)(int timeout);
typedef void (TYPEOF_jtrdll_get_status)(struct JTRDLL_STATUS *jtrdllstatus);
typedef int (TYPEOF_jtrdll_get_charset_info)(const char *path, unsigned char * charmin, unsigned char *charmax, unsigned char *len, unsigned char *count, unsigned char allchars[256]);
typedef void (TYPEOF_jtrdll_cleanup)(void);
typedef void (TYPEOF_jtrdll_preflight)(int argc, char **argv, struct JTRDLL_HOOKS *hooks, struct JTRDLL_PREFLIGHT *jtrdllpreflight);
typedef void (TYPEOF_jtrdll_set_extra_opencl_kernel_args)(const char *args);

#ifdef JTRDLL
extern char extra_opencl_kernel_args[1024];
extern int jtrdll_is_preflight;
extern unsigned long jtrdll_preflight_salt_count;
extern unsigned long jtrdll_preflight_wordlist_rule_count;
//extern unsigned long long jtrdll_preflight_wordlist_candidates;
extern unsigned long long jtrdll_preflight_mask_candidates;
extern unsigned long long jtrdll_preflight_incremental_candidates;
#endif

#ifdef __cplusplus
}
#endif


#endif