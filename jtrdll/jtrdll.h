#ifndef __INC_JTRDLL_H
#define __INC_JTRDLL_H

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

struct JTRDLL_STATUS
{
	double percent;
	unsigned int time;
	unsigned int eta;
	unsigned long long guess_count;
	unsigned long long candidates;
	unsigned long long guesses_per_second;
	unsigned long long candidates_per_second;
	unsigned long long crypts_per_second;
	unsigned long long combinations_per_second;
	char word1[0x80];
	char word2[0x80];
};

struct JTRDLL_HOOKS 
{
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
JTRDLL_IMPEXP void jtrdll_abort(void);
#endif

typedef int (TYPEOF_jtrdll_main)(int argc, char **argv, struct JTRDLL_HOOKS *hooks);
typedef void (TYPEOF_jtrdll_abort)(void);
typedef void (TYPEOF_jtrdll_get_status)(struct JTRDLL_STATUS *jtrdllstatus);

#ifdef __cplusplus
}
#endif


#endif