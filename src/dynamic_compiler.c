/*
 * This software was written by Jim Fougeron jfoug AT cox dot net
 * in 2015. No copyright is claimed, and the software is hereby
 * placed in the public domain. In case this attempt to disclaim
 * copyright and place the software in the public domain is deemed
 * null and void, then the software is Copyright (c) 2015 Jim Fougeron
 * and it is hereby released to the general public under the following
 * terms:
 *
 * This software may be modified, redistributed, and used for any
 * purpose, in source and binary forms, with or without modification.
 *
 * Expression to Generic 'scriptable' compiler/builder for the
 * existing dynamic format.
 *
 *  Here is an 'expression'
 *    dynamic=EXPR[,param,param....]
 *    This expression language will be very similar to the expression language in pass_gen.pl
 *
 *  Valid items in EXPR
 *     md5(EXPR)   Perform MD5.   Results in lowcase hex (unless it's the outter EXPR)
 *     sha1(EXPR)  Perform SHA1.  Results in lowcase hex (unless it's the outter EXPR)
 *     md4(EXPR), sha256(EXPR), sha224(EXPR), sha384(EXPR), sha512(EXPR), gost(EXPR),
 *     whirlpool(EXPR) tiger(EXPR), ripemd128(EXPR), ripemd160(EXPR), ripemd256(EXPR),
 *     ripemd320(EXPR) all act like md5() and sha1, but use the hash listed.
 *
 *     MD5(EXPR) (and other hashes with upper case string), is the same as the lower
 *     case variant, except the hex result will be upper cased.
 *
 *     From here on ONLY md5() will be used to explain the expression language, BUT
 *     the md5 can be replaced with any of the other hash types.
 *
 *     md5_raw(EXPR) this one returns the raw buffer from the md5 (the 16 byte buffer)
 *     Other hashes will return their raw buffer (all valid bytes which depend upon hash)
 *
 *     md5_64(EXPR) This one returns mime base-64,
 *
 *     md5_64c(EXPR)  This one returns crypt base-64
 *
 *     $p   The input password
 *
 *     .    The . char means concatination.  So $p.$p will be the password concatenated
 *          to itself.  $p.md5($p.$s).$s.$p is the password concated with the md5 of the
 *          password concatentated to the salt. This result is then concated with the salt
 *          and password again.  Dots are required.  md5(md5($p)md5($p)) is not valid, but
 *          md5(md5($p).md5($p)) is the proper syntax.
 *
 *  params:
 *   (Note, to have a comma IN a param value, use \, )
 *   (Note all params a comma separated. The last param can have the comma omitted)
 *
 *     c1=const_value    Up to 9 const values. they must be 'packed', i.e. if you have
 *     ...               2 of the, then you MUST use c1=something,c2=somethingelse
 *     c9=const_value.
 *     pass=uni          Unicode the passwords
 *     passcase=[L|U]    This will up case or low case the password
 *     salt=             (true), ashex or tohex
 *     usrname=          (true), lc, uc, uni
 *     saltlen=#         This sets the length of the salt
 *     debug             If this is set, then JtR will output the script and other data and exit.
 *     optimize          If set, performs optimizations
 *     optimize2         If set, performs 2nd level of optimizations.
 *
 *****************************************************************
 *     TODO:
 *****************************************************************

 add new logic for ^  (exponentiation)

 Handle:
#define MGF_KEYS_INPUT                   0x00000001
#define MGF_KEYS_CRYPT_IN2               0x00000002
// for salt_as_hex for other formats, we do this:  (flag>>56)
// Then 00 is md5, 01 is md4, 02 is SHA1, etc
// NOTE, all top 8 bits of the flags are reserved, and should NOT be used for flags.
#define MGF_KEYS_BASE16_IN1              0x00000004   // deprecated (use the _MD5 version)
#define MGF_KEYS_BASE16_IN1_MD5          0x0000000000000004ULL
#define MGF_KEYS_BASE16_IN1_MD4	         0x0100000000000004ULL
#define MGF_KEYS_BASE16_IN1_SHA1         0x0200000000000004ULL
#define MGF_KEYS_BASE16_IN1_SHA224       0x0300000000000004ULL
#define MGF_KEYS_BASE16_IN1_SHA256       0x0400000000000004ULL
#define MGF_KEYS_BASE16_IN1_SHA384       0x0500000000000004ULL
#define MGF_KEYS_BASE16_IN1_SHA512       0x0600000000000004ULL
#define MGF_KEYS_BASE16_IN1_GOST         0x0700000000000004ULL
#define MGF_KEYS_BASE16_IN1_WHIRLPOOL    0x0800000000000004ULL
#define MGF_KEYS_BASE16_IN1_Tiger        0x0900000000000004ULL
#define MGF_KEYS_BASE16_IN1_RIPEMD128    0x0A00000000000004ULL
#define MGF_KEYS_BASE16_IN1_RIPEMD160    0x0B00000000000004ULL
#define MGF_KEYS_BASE16_IN1_RIPEMD256    0x0C00000000000004ULL
#define MGF_KEYS_BASE16_IN1_RIPEMD320    0x0D00000000000004ULL

#define MGF_KEYS_BASE16_IN1_Offset32         0x00000008   // deprecated (use the _MD5 version)
#define MGF_KEYS_BASE16_IN1_Offset_MD5       0x0000000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_MD4       0x0100000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_SHA1      0x0200000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_SHA224    0x0300000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_SHA256    0x0400000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_SHA384    0x0500000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_SHA512    0x0600000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_GOST      0x0700000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_WHIRLPOOL 0x0800000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_Tiger     0x0900000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_RIPEMD128 0x0A00000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_RIPEMD160 0x0B00000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_RIPEMD256 0x0C00000000000008ULL
#define MGF_KEYS_BASE16_IN1_Offset_RIPEMD320 0x0D00000000000008ULL

BENCHMARK_FLAG  (for raw hashes, etc).

// MGF_INPBASE64 uses e_b64_cryptBS from base64_convert.h chang b64e to b64c
#define MGF_INPBASE64		         0x00000080
if any utf16 used, set this flag??

Figure out how to better hook in dynamic= so that we can use this in other
situations than ONLY working if -format=dynamic=expr is used.

Right now, all handles are allocated 'tiny'.  Change that so that we
do normal alloc/calloc, keep a list of all handles ever allocated, and
then upon void dynamic_compile_done() free up all memory from that list.

$p and or uc($p) or lc($p) MUST be the only way $p is shown. If uc($p) then ALL must be uc, etc.
Same goes for $u.  NOTE, the lexi works this way. I just need to document it.

DONE // MGF_INPBASE64b uses e_b64_crypt from base64_convert.h
DONE #define MGF_INPBASE64b		         0x00004000
DONE if outter hash is md5_b64 (or b64e) then use this flag
DONE #define MGF_INPBASE64m               0x02000000
DONE #define MGF_UTF8                     0x04000000
DONE Remove all md5u() types.  Replace with a utf16() function.
DONE #define MGF_USERNAME_UPCASE         (0x00000020|MGF_USERNAME)
DONE #define MGF_USERNAME_LOCASE         (0x00000040|MGF_USERNAME)
DONE #define MGF_PASSWORD_UPCASE          0x08000000
DONE #define MGF_PASSWORD_LOCASE          0x10000000
DONE #define MGF_BASE_16_OUTPUT_UPCASE    0x00002000

 */

#include "arch.h"

#ifndef DYNAMIC_DISABLED
#include <ctype.h>
#include <stdarg.h>
#include "common.h"
#include "stdint.h"
#include "formats.h"
#include "list.h"
#include "crc32.h"
#include "johnswap.h"
#include "dynamic.h"
#include "dynamic_compiler.h"
#include "base64_convert.h"
#include "md5.h"
#include "md4.h"
#include "sha.h"
#include "sha2.h"
#include "gost.h"
#include "unicode.h"
// this one is going to be harder.  only haval_256_5 is implemented in CPAN perl, making generation of test cases harder.
// Also, there are 15 different hashes in this 'family'.
//#include "sph_haval.h"

#include "sph_ripemd.h"
#include "sph_tiger.h"
#include "sph_whirlpool.h"
#include "sph_haval.h"
#include "sph_md2.h"
#include "sph_panama.h"
#include "sph_skein.h"

#if (AC_BUILT && HAVE_WHIRLPOOL) ||	  \
   (!AC_BUILT && OPENSSL_VERSION_NUMBER >= 0x10000000 && !HAVE_NO_SSL_WHIRLPOOL)
#include <openssl/whrlpool.h>
#else
// on my 32 bit cygwin builds, this code is about 4x slower than the oSSL code.
#define WHIRLPOOL_CTX             sph_whirlpool_context
#define WHIRLPOOL_Init(a)         sph_whirlpool_init(a)
#define WHIRLPOOL_Update(a,b,c)   sph_whirlpool(a,b,c)
#define WHIRLPOOL_Final(a,b)      sph_whirlpool_close(b,a)
#endif

#include "memdbg.h"

static int gost_init = 0;
const char *options_format="";

typedef struct DC_list {
	struct DC_list *next;
	DC_struct *value;
} DC_list;

const char *dyna_script = "Expression=dynamic=md5($p)\nFlag=MGF_KEYS_INPUT\nFunc=DynamicFunc__crypt_md5\nTest=@dynamic=md5($p)@900150983cd24fb0d6963f7d28e17f72:abc";
const char *dyna_signature = "@dynamic=md5($p)@";
const char *dyna_line1 = "@dynamic=md5($p)@900150983cd24fb0d6963f7d28e17f72";
const char *dyna_line2 = "@dynamic=md5($p)@527bd5b5d689e2c32ae974c6229ff785";
const char *dyna_line3 = "@dynamic=md5($p)@9dc1dc3f8499ab3bbc744557acf0a7fb";
int dyna_sig_len = 17;

extern char *dynamic_Demangle(char*,int *);

#ifdef WITH_MAIN
int GEN_BIG=0;
#endif

static DC_list *pList;
static DC_struct *pLastFind;

static uint32_t compute_checksum(const char *expr);
static DC_HANDLE find_checksum(uint32_t crc32);
static DC_HANDLE do_compile(const char *expr, uint32_t crc32);
static void add_checksum_list(DC_HANDLE pHand);

// TODO
static char *dynamic_expr_normalize(const char *ct) {
	// normalize $pass -> $p
	//           $password -> $p
	//           $salt -> $s
	//           $user -> $u
	//           $username -> $u
	//           unicode( -> utf16(
	if (/*!strncmp(ct, "@dynamic=", 9) &&*/ (strstr(ct, "$pass") || strstr(ct, "$salt") || strstr(ct, "$user"))) {
		static char Buf[512];
		char *cp = Buf;
		strcpy(Buf, ct);
		ct = Buf;
		cp = Buf;
		while (*cp) {
			int cnt=0;
			while (*cp && *cp != '$' && *cp != 'u')
				++cp;
			if (*cp) {
				if (!strncmp(cp, "$password", 9))
					cnt = 7;
				else if (!strncmp(cp, "$pass", 5))
					cnt = 3;
				else if (!strncmp(cp, "$salt", 5))
					cnt = 3;
				else if (!strncmp(cp, "$username", 9))
					cnt = 7;
				else if (!strncmp(cp, "$user", 5))
					cnt = 3;
				else if (!strncmp(cp, "unicode(", 8)) {
					memcpy(cp, "utf16", 5);
					cp += 3;
					cnt = 2;
				}
			}
			cp += 2;;
			if (cnt) {
				char *cp2 = cp;
				while (cp2[cnt]) {
					*cp2 = cp2[cnt];
					++cp2;
				}
				*cp2 = 0;
			}
		}
	}
	//
	// TODO:  put in this order:  expression,c1=,c2=,...,cn=,passcase=,saltlen=,pass=uni,salt=,usrname=,debug,optimize,optimize2
	// NOTE,  we only crc up to the 'passcase=' part.  Everything after that should not be used to 'distinguish' the format hash
	//        between hashes.  NOTE, it may still cause valid to not 'load' for testing some hashes, but these still should
	//        be loaded from the .pot file to cross off items.
	//
	return (char*)ct;
}

int dynamic_compile(const char *expr, DC_HANDLE *p) {
	uint32_t crc32 = compute_checksum(dynamic_expr_normalize(expr));
	DC_HANDLE pHand;
	if (pLastFind && pLastFind->crc32 == crc32) {
		*p = (DC_HANDLE)pLastFind;
		return 0;
	}

	pHand = find_checksum(crc32);
	if (pHand) {
		*p = pHand;
		pLastFind = (DC_struct*)pHand;
		return 0;
	}
	/* this is the real 'workhorse' function */
	pHand = do_compile(expr, crc32);
	if (!pHand)
		return 1;
	add_checksum_list(pHand);
	*p = pHand;
	return 0;
}

static char *find_the_expression(const char *expr) {
	static char buf[512];
	char *cp;
	if (strncmp(expr, "dynamic=", 8))
		return "";
	strnzcpy(buf, &expr[8], sizeof(buf));
	cp = strrchr(buf, ')');
	if (!cp) return "";
	while (cp[1] && cp[1] != ',')
		++cp;
	cp[1] = 0;
	cp = dynamic_expr_normalize(buf);
	return cp;
}
static char *find_the_extra_params(const char *expr) {
	static char buf[512];
	char *cp;
	if (strncmp(expr, "dynamic=", 8))
		return "";
	cp = strrchr(expr, ')');
	if (!cp) return "";
	if (cp[1] == ',') ++cp;
	strnzcpy(buf, &cp[1], sizeof(buf));
	// NOTE, we should normalize this string!!
	// we should probably call handle_extra_params, and then make a function
	// regen_extra_params() so that we always normalize this string.
	return buf;
}

#define ARRAY_COUNT(a) (sizeof(a)/sizeof(a[0]))
typedef void (*fpSYM)();
static int nConst;
static const char *Const[9];
static int compile_debug;
static char *SymTab[1024];
static fpSYM fpSymTab[1024];
static int nSymTabLen[1024];
static char *pCode[1024];
static fpSYM fpCode[1024];
static int nLenCode[1024];
static int nCode, nCurCode;
static char *pScriptLines[1024];
static int nScriptLines;
static int outer_hash_len;
static int nSyms;
static int LastTokIsFunc;
static int bNeedS, bNeedS2, bNeedU, bNeedUlc, bNeedUuc, bNeedPlc, bNeedPuc, bNeedUC, bNeedPC;
static char *salt_as_hex_type;
static int keys_as_input;
static char *gen_Stack[1024];
static int gen_Stack_len[1024];
static int ngen_Stack, ngen_Stack_max;
static char *h;
static int h_len;
static int nSaltLen = -32;
static char gen_s[260], gen_s2[16], gen_u[16], gen_uuc[16], gen_ulc[16], gen_pw[16], gen_puc[16], gen_plc[16], gen_conv[260];

/*
 * These are the 'low level' primative functions ported from pass_gen.pl.
 * These do the md5($p) stuff (hex, HEX, unicode, base64, etc), for all hash
 * types, and for other functions.
 * now done using macros (makes source smaller and easier to maintain). NOTE
 * it is not very debuggable, but that really should not matter.' I would rather
 * have working prototypes like this, then simply add 1 line to get a NEW full
 * set of properly working functions.
 */
#define OSSL_FUNC(N,TT,T,L) \
static void N##_hex()    {TT##_CTX c; T##_Init(&c); T##_Update(&c,h,h_len); T##_Final((unsigned char*)h,&c); base64_convert(h,e_b64_raw,L,gen_conv,e_b64_hex,260,0); strcpy(h, gen_conv); } \
static void N##_base64() {TT##_CTX c; T##_Init(&c); T##_Update(&c,h,h_len); T##_Final((unsigned char*)h,&c); base64_convert(h,e_b64_raw,L,gen_conv,e_b64_mime,260,0); strcpy(h, gen_conv); } \
static void N##_base64c(){TT##_CTX c; T##_Init(&c); T##_Update(&c,h,h_len); T##_Final((unsigned char*)h,&c); base64_convert(h,e_b64_raw,L,gen_conv,e_b64_crypt,260,0); strcpy(h, gen_conv); } \
static void N##_raw()    {TT##_CTX c; T##_Init(&c); T##_Update(&c,h,h_len); T##_Final((unsigned char*)h,&c); }
OSSL_FUNC(md5,MD5,MD5,16)
OSSL_FUNC(md4,MD4,MD4,16)
OSSL_FUNC(sha1,SHA,SHA1,20)
OSSL_FUNC(sha224,SHA256,SHA224,28)
OSSL_FUNC(sha256,SHA256,SHA256,32)
OSSL_FUNC(sha384,SHA512,SHA384,48)
OSSL_FUNC(sha512,SHA512,SHA512,64)
OSSL_FUNC(whirlpool,WHIRLPOOL,WHIRLPOOL,64)
static void gost_hex()         { gost_ctx c; john_gost_init(&c); john_gost_update(&c, (unsigned char*)h, h_len); john_gost_final(&c, (unsigned char*)h); base64_convert(h,e_b64_raw,32,gen_conv,e_b64_hex,260,0); strcpy(h, gen_conv); }
static void gost_base64()      { gost_ctx c; john_gost_init(&c); john_gost_update(&c, (unsigned char*)h, h_len); john_gost_final(&c, (unsigned char*)h); base64_convert(h,e_b64_raw,32,gen_conv,e_b64_mime,260,0); strcpy(h, gen_conv); }
static void gost_base64c()     { gost_ctx c; john_gost_init(&c); john_gost_update(&c, (unsigned char*)h, h_len); john_gost_final(&c, (unsigned char*)h); base64_convert(h,e_b64_raw,32,gen_conv,e_b64_crypt,260,0); strcpy(h, gen_conv); }
static void gost_raw()         { gost_ctx c; john_gost_init(&c); john_gost_update(&c, (unsigned char*)h, h_len); john_gost_final(&c, (unsigned char*)h); }
#define SPH_FUNC(T,L) \
static void T##_hex()    { sph_##T##_context c; sph_##T##_init(&c); sph_##T(&c, (unsigned char*)h, h_len); sph_##T##_close(&c, (unsigned char*)h); base64_convert(h,e_b64_raw,L,gen_conv,e_b64_hex,260,0); strcpy(h, gen_conv); } \
static void T##_base64() { sph_##T##_context c; sph_##T##_init(&c); sph_##T(&c, (unsigned char*)h, h_len); sph_##T##_close(&c, (unsigned char*)h); base64_convert(h,e_b64_raw,L,gen_conv,e_b64_mime,260,0); strcpy(h, gen_conv); } \
static void T##_base64c(){ sph_##T##_context c; sph_##T##_init(&c); sph_##T(&c, (unsigned char*)h, h_len); sph_##T##_close(&c, (unsigned char*)h); base64_convert(h,e_b64_raw,L,gen_conv,e_b64_crypt,260,0); strcpy(h, gen_conv); } \
static void T##_raw()    { sph_##T##_context c; sph_##T##_init(&c); sph_##T(&c, (unsigned char*)h, h_len); sph_##T##_close(&c, (unsigned char*)h); }
SPH_FUNC(tiger,24)
SPH_FUNC(ripemd128,16)  SPH_FUNC(ripemd160,20)  SPH_FUNC(ripemd256,32) SPH_FUNC(ripemd320,40)
SPH_FUNC(haval128_3,16) SPH_FUNC(haval128_4,16) SPH_FUNC(haval128_5,16)
SPH_FUNC(haval160_3,20) SPH_FUNC(haval160_4,20) SPH_FUNC(haval160_5,20)
SPH_FUNC(haval192_3,24) SPH_FUNC(haval192_4,24) SPH_FUNC(haval192_5,24)
SPH_FUNC(haval224_3,28) SPH_FUNC(haval224_4,28) SPH_FUNC(haval224_5,28)
SPH_FUNC(haval256_3,32) SPH_FUNC(haval256_4,32) SPH_FUNC(haval256_5,32)
SPH_FUNC(md2,16) SPH_FUNC(panama,32)
SPH_FUNC(skein224,28) SPH_FUNC(skein256,32) SPH_FUNC(skein384,48) SPH_FUNC(skein512,64)

static int encode_le()         { int len = enc_to_utf16((UTF16*)gen_conv, 260, (UTF8*)h, h_len); memcpy(h, gen_conv, len*2); return len*2; }
static char *pad16()           { memset(gen_conv, 0, 16); strncpy(gen_conv, gen_pw, 16); return gen_conv; }
static char *pad20()           { memset(gen_conv, 0, 20); strncpy(gen_conv, gen_pw, 20); return gen_conv; }
static char *pad100()          { memset(gen_conv, 0, 100); strncpy(gen_conv, gen_pw, 100); return gen_conv; }

// TODO:
//int encode_be() { int len = enc_to_utf16_be((UTF16*)gen_conv, 260, (UTF8*)h, h_len); memcpy(h, gen_conv, len); return len; }

/*
 * helper functions, to reduce the size of our dynamic_*() functions
 */
static void dyna_helper_append(const char *v) {
	memcpy(&gen_Stack[ngen_Stack-1][gen_Stack_len[ngen_Stack-1]], v, strlen(v));
	gen_Stack_len[ngen_Stack-1] += strlen(v);
}
static void dyna_helper_appendn(const char *v, int len) {
	memcpy(&gen_Stack[ngen_Stack-1][gen_Stack_len[ngen_Stack-1]], v, len);
	gen_Stack_len[ngen_Stack-1] += len;
}
static void dyna_helper_pre() {
	h = gen_Stack[--ngen_Stack];
	h_len = gen_Stack_len[ngen_Stack];
}
static void dyna_helper_post(int len) {
	// like dyna_helper_append but always works with the h variable.
	// this one is for binary data, so we have to pass the length in.
	memcpy(&gen_Stack[ngen_Stack-1][gen_Stack_len[ngen_Stack-1]], h, len);
	gen_Stack_len[ngen_Stack-1] += len;
}
static void dyna_helper_poststr() {
	// like dyna_helper_append but always works with the h variable.
	int len = strlen(h);
	memcpy(&gen_Stack[ngen_Stack-1][gen_Stack_len[ngen_Stack-1]], h, len);
	gen_Stack_len[ngen_Stack-1] += len;
}

/*
 * these are the functions called by the script. They may do all the work
 * themselves, or may also use the low level primatives for hashing.
 */
static void fpNull(){}
static void dynamic_push()   { char *p = mem_calloc(260, 1); MEM_FREE(gen_Stack[ngen_Stack]); gen_Stack_len[ngen_Stack] = 0; gen_Stack[ngen_Stack++] = p; ngen_Stack_max++; }
static void dynamic_pad16()  { dyna_helper_appendn(pad16(), 16);  }
static void dynamic_pad20()  { dyna_helper_appendn(pad20(), 20);  }
static void dynamic_pad100() { dyna_helper_appendn(pad100(), 100); }
//static void dynamic_pop    { return pop @gen_Stack; }  # not really needed.

#define APP_FUNC(TY,VAL) static void dynamic_app_##TY (){dyna_helper_append(VAL);}
APP_FUNC(sh,gen_s) /*APP_FUNC(s,gen_s)*/ APP_FUNC(S,gen_s2) APP_FUNC(u,gen_u) APP_FUNC(u_lc,gen_ulc)
APP_FUNC(u_uc,gen_uuc) APP_FUNC(p,gen_pw) APP_FUNC(p_uc,gen_puc) APP_FUNC(p_lc,gen_plc)
#define APP_CFUNC(N) static void dynamic_app_##N (){dyna_helper_append(dynamic_Demangle((char*)Const[N],NULL));}
APP_CFUNC(1) APP_CFUNC(2) APP_CFUNC(3) APP_CFUNC(4) APP_CFUNC(5) APP_CFUNC(6) APP_CFUNC(7) APP_CFUNC(8)
//static void dynamic_ftr32  { $h = gen_Stack[--ngen_Stack]; substr($h,0,32);  strcat(gen_Stack[ngen_Stack-1], h);  }
//static void dynamic_f54    { $h = gen_Stack[--ngen_Stack]; md5_hex(h)."00000000";	 strcat(gen_Stack[ngen_Stack-1], h);  }

#define LEXI_FUNC(N,T,L) \
	static void dynamic_f##N##h()    { dyna_helper_pre(); T##_hex();               dyna_helper_poststr(); } \
	static void dynamic_f##N##H()    { dyna_helper_pre(); T##_hex(); strupr(h);    dyna_helper_poststr(); } \
	static void dynamic_f##N##6()    { dyna_helper_pre(); T##_base64();	           dyna_helper_poststr(); } \
	static void dynamic_f##N##c()    { dyna_helper_pre(); T##_base64c();           dyna_helper_poststr(); } \
	static void dynamic_f##N##r()    { dyna_helper_pre(); T##_raw();               dyna_helper_post(L); }
LEXI_FUNC(5,md5,16)       LEXI_FUNC(4,md4,16)          LEXI_FUNC(1,sha1,20)
LEXI_FUNC(224,sha224,28)  LEXI_FUNC(256,sha256,32)     LEXI_FUNC(384,sha384,48)  LEXI_FUNC(512,sha512,64)
LEXI_FUNC(gost,gost,28)   LEXI_FUNC(tig,tiger,24)      LEXI_FUNC(wrlp,whirlpool,64)
LEXI_FUNC(rip128,ripemd128,16) LEXI_FUNC(rip160,ripemd160,20) LEXI_FUNC(rip256,ripemd256,32) LEXI_FUNC(rip320,ripemd320,40)
LEXI_FUNC(hav128_3,haval128_3,16) LEXI_FUNC(hav128_4,haval128_4,16) LEXI_FUNC(hav128_5,haval128_5,16)
LEXI_FUNC(hav160_3,haval160_3,20) LEXI_FUNC(hav160_4,haval160_4,20) LEXI_FUNC(hav160_5,haval160_5,20)
LEXI_FUNC(hav192_3,haval192_3,24) LEXI_FUNC(hav192_4,haval192_4,24) LEXI_FUNC(hav192_5,haval192_5,24)
LEXI_FUNC(hav224_3,haval224_3,28) LEXI_FUNC(hav224_4,haval224_4,28) LEXI_FUNC(hav224_5,haval224_5,28)
LEXI_FUNC(hav256_3,haval256_3,32) LEXI_FUNC(hav256_4,haval256_4,32) LEXI_FUNC(hav256_5,haval256_5,32)
LEXI_FUNC(md2,md2,16) LEXI_FUNC(pan,panama,32)
LEXI_FUNC(skn224,skein224,28) LEXI_FUNC(skn256,skein256,32) LEXI_FUNC(skn384,skein384,48) LEXI_FUNC(skn512,skein512,64)

static void dynamic_futf16()    { dyna_helper_pre();                             dyna_helper_post(encode_le()); }
//static void dynamic_futf16be()  { dyna_helper_pre();                             dyna_helper_post(encode_be()); }
static void dynamic_exp() {
	int i, j;
	j = atoi(&pCode[nCurCode][1]);
	for (i = 1; i < j; ++i) {
		gen_Stack_len[ngen_Stack] = gen_Stack_len[ngen_Stack-1];
		gen_Stack_len[ngen_Stack-1] = 0;
		++ngen_Stack;
		fpCode[nCurCode-1]();
	}
}

static void init_static_data() {
	int i;
	nConst = 0;
	for (i = 0; i < nSyms; ++i) {
		MEM_FREE(SymTab[i]);
		fpSymTab[i] = NULL;
		nSymTabLen[i] = 0;
	}
	for (i = 0; i < 9; ++i) {
		if (Const[i]) {
			char *p = (char*)Const[i];
			MEM_FREE(p);
		}
		Const[i] = NULL;
	}
	for (i = 0; i < nCode; ++i) {
		MEM_FREE(pCode[i]);
		fpCode[i] = NULL;
		nLenCode[i] = 0;
	}
	for (i = 0; i < nScriptLines; ++i)
		MEM_FREE(pScriptLines[i]);
	for (i = 0; i < ngen_Stack; ++i) {
		MEM_FREE(gen_Stack[i]);
		gen_Stack_len[i] = 0;
	}
	ngen_Stack = ngen_Stack_max = 0;
	nCode = nCurCode = 0;
	nSyms = 0;
	nScriptLines = 0;
	LastTokIsFunc = 0;
	keys_as_input = 0;
	outer_hash_len = 0;
	bNeedS = bNeedS2 = bNeedU = bNeedUlc = bNeedUuc = bNeedPlc = bNeedPuc = bNeedUC = bNeedPC = compile_debug = 0;
	MEM_FREE(salt_as_hex_type);
	h = NULL;
	h_len = 0;
	nSaltLen = -32;
	memset(gen_s, 0, sizeof(gen_s));
	memset(gen_s2, 0, sizeof(gen_s2));
	memset(gen_u, 0, sizeof(gen_u));
	memset(gen_uuc, 0, sizeof(gen_uuc));
	memset(gen_ulc, 0, sizeof(gen_ulc));
	memset(gen_pw, 0, sizeof(gen_pw));
	memset(gen_puc, 0, sizeof(gen_puc));
	memset(gen_plc, 0, sizeof(gen_plc));
	memset(gen_conv, 0, sizeof(gen_conv));
}
static const char *get_param(const char *p, const char *what) {
	const char *cp;
	char *cpRet;
	p = strstr(p, what);
	if (!p)
		return NULL;
	p += strlen(what);
	cp = strchr(p, ',');
	while (cp && cp[-1] == '\\')
		cp = strchr(&cp[1], ',');
	if (cp) {
		cpRet = mem_alloc_tiny((cp-p)+1, 1);
		memcpy(cpRet, p, cp-p);
		cpRet[cp-p] = 0;
	} else {
		cpRet = mem_alloc_tiny(strlen(p)+1, 1);
		strcpy(cpRet, p);
	}
	return cpRet;
}
static int handle_extra_params(DC_struct *ptr) {
	// c1=boobies,c2=bootie
	int i;
	char cx[4];
	const char *cp;

	nConst = 0;
	if (!ptr->pExtraParams || !ptr->pExtraParams[0])
		return 0;

	// Find any 'const' values that have been provided.
	for (i = 1; i < 9; ++i) {
		char *cp2;
		sprintf(cx, "c%d=", i);
		cp = get_param(ptr->pExtraParams, cx);
		if (!cp)
			break;

		cp2 = mem_alloc(strlen(cp)+1);
		strcpy(cp2, cp);
		Const[++nConst] = cp2;
	}

	// Find any other values here.
	if (strstr(ptr->pExtraParams, "debug,") || strstr(ptr->pExtraParams, ",debug") || !strcmp(ptr->pExtraParams, "debug"))
		compile_debug = 1;

	if ( (cp = get_param(ptr->pExtraParams, "saltlen")) != NULL) {
		nSaltLen = atoi(&cp[1]);
		if (nSaltLen > 200)
			error("Max salt len allowed is 200 bytes\n");
	}
	return 0;
}

static const char *comp_push_sym(const char *p, fpSYM fpsym, const char *pRet, int len) {
	if (nSyms < ARRAY_COUNT(SymTab)) {
		SymTab[nSyms] = mem_alloc(strlen(p)+1);
		fpSymTab[nSyms] = fpsym;
		nSymTabLen[nSyms] = len;
		strcpy(SymTab[nSyms++], p);
	}
	return pRet;
}

#define LOOKUP_IF_BLK(T,U,S,F,L,LL) \
	if (!strncasecmp(pInput, #T, L)) { \
		if (!strncmp(pInput, #T "_raw(", L+5)) { LastTokIsFunc = 2; return comp_push_sym("f" #S "r", dynamic_f##F##r, pInput+(L+4), LL); } \
		if (!strncmp(pInput, #T "_64c(", L+5)) { return comp_push_sym("f" #S "c", dynamic_f##F##c, pInput+(L+4), LL); } \
		if (!strncmp(pInput, #T "_64(", L+4)) { return comp_push_sym("f" #S "6", dynamic_f##F##6, pInput+(L+3), LL); } \
		if (!strncmp(pInput, #T "(", L+1)) { return comp_push_sym("f" #S "h", dynamic_f##F##h, pInput+L, LL); } \
		if (!strncmp(pInput, #U "(", L+1)) { return comp_push_sym("f" #S "H", dynamic_f##F##H, pInput+L, LL); } }

static const char *comp_get_symbol(const char *pInput) {
	// This function will grab the next valid symbol, and returns
	// the location just past this symbol.
	char TmpBuf[64];
	LastTokIsFunc = 0;
	if (!pInput || *pInput == 0) return comp_push_sym("X", fpNull, pInput, 0);
	if (*pInput == '.') return comp_push_sym(".", fpNull, pInput+1, 0);
	if (*pInput == '(') return comp_push_sym("(", fpNull, pInput+1, 0);
	if (*pInput == ')') return comp_push_sym(")", fpNull, pInput+1, 0);
	if (*pInput == '^') {
		int i=1;
		// number. Can follow a ^, like    md5_raw($p)^5   ==  md5(md5(md5(md5(md5($p)))))
		*TmpBuf = '^';
		if (!isdigit(ARCH_INDEX(pInput[1])))
			return comp_push_sym("X", fpNull, pInput+1, 0);
		while (i < 10 && isdigit(ARCH_INDEX(pInput[i])))
			++i;
		memcpy(&TmpBuf[1], &pInput[1], i-1);
		TmpBuf[i]=0;
		return comp_push_sym(TmpBuf, fpNull, pInput+i, 0);
	}
	if (*pInput == '$') {
		switch(pInput[1]) {
			case 'p': { if (bNeedPC>1) return comp_push_sym("X", fpNull, pInput, 0); bNeedPC=-1; return comp_push_sym("p", fpNull, pInput+2, 0); }
			case 'u': { bNeedU=1; if (bNeedUC>0) return comp_push_sym("X", fpNull, pInput, 0); bNeedUC=-1; return comp_push_sym("u", fpNull, pInput+2, 0); }
			case 's': if (pInput[2] == '2') return comp_push_sym("S", fpNull, pInput+3, 0);
					  return comp_push_sym("s", fpNull, pInput+2, 0);
			case 'c': if (pInput[2] > '8' || pInput[2] < '1')
						  return comp_push_sym("X", fpNull, pInput, 0);
					  if (Const[pInput[2]-'0'] == NULL) {
						  fprintf(stderr, "Error, a c%c found in expression, but the data for this const was not provided\n", pInput[2]);
						  return comp_push_sym("X", fpNull, pInput, 0);
					  }
					  TmpBuf[0] = pInput[2];
					  TmpBuf[1] = 0;
					  return comp_push_sym(TmpBuf, fpNull, pInput+3, 0);
		}
	}
	// these are functions, BUT can not be used for 'outter' function (i.e. not the final hash)
	// Note this may 'look' small, but it is a large IF block, once the macro's expand
	LastTokIsFunc = 1;
	LOOKUP_IF_BLK(md5,MD5,5,5,3,16)
	LOOKUP_IF_BLK(md4,MD4,4,4,3,16)
	LOOKUP_IF_BLK(sha1,SHA1,1,1,4,20)
	LOOKUP_IF_BLK(sha224,SHA224,224,224,6,28)
	LOOKUP_IF_BLK(sha256,SHA256,256,256,6,32)
	LOOKUP_IF_BLK(sha384,SHA384,384,384,6,48)
	LOOKUP_IF_BLK(sha512,SHA512,512,512,6,64)
	LOOKUP_IF_BLK(gost,GOST,gost,gost,4,32)
	LOOKUP_IF_BLK(tiger,TIGER,tig,tig,5,24)
	LOOKUP_IF_BLK(whirlpool,WHIRLPOOL,wrlp,wrlp,9,64)
	LOOKUP_IF_BLK(ripemd128,RIPEMD128,rip128,rip128,9,16) 	LOOKUP_IF_BLK(ripemd160,RIPEMD160,rip160,rip160,9,20)
	LOOKUP_IF_BLK(ripemd256,RIPEMD256,rip256,rip256,9,32)	LOOKUP_IF_BLK(ripemd320,RIPEMD320,rip320,rip320,9,40)
	LOOKUP_IF_BLK(haval128_3,HAVAL128_3,hav128_3,hav128_3,10,16) LOOKUP_IF_BLK(haval128_4,HAVAL128_4,hav128_4,hav128_4,10,16) LOOKUP_IF_BLK(haval128_5,HAVAL128_5,hav128_5,hav128_5,10,16)
	LOOKUP_IF_BLK(haval160_3,HAVAL160_3,hav160_3,hav160_3,10,20) LOOKUP_IF_BLK(haval160_4,HAVAL160_4,hav160_4,hav160_4,10,20) LOOKUP_IF_BLK(haval160_5,HAVAL160_5,hav160_5,hav160_5,10,20)
	LOOKUP_IF_BLK(haval192_3,HAVAL192_3,hav192_3,hav192_3,10,24) LOOKUP_IF_BLK(haval192_4,HAVAL192_4,hav192_4,hav192_4,10,24) LOOKUP_IF_BLK(haval192_5,HAVAL192_5,hav192_5,hav192_5,10,24)
	LOOKUP_IF_BLK(haval224_3,HAVAL224_3,hav224_3,hav224_3,10,28) LOOKUP_IF_BLK(haval224_4,HAVAL224_4,hav224_4,hav224_4,10,28) LOOKUP_IF_BLK(haval224_5,HAVAL224_5,hav224_5,hav224_5,10,28)
	LOOKUP_IF_BLK(haval256_3,HAVAL256_3,hav256_3,hav256_3,10,32) LOOKUP_IF_BLK(haval256_4,HAVAL256_4,hav256_4,hav256_4,10,32) LOOKUP_IF_BLK(haval256_5,HAVAL256_5,hav256_5,hav256_5,10,32)
	LOOKUP_IF_BLK(md2,MD2,md2,md2,3,16) LOOKUP_IF_BLK(panama,PANAMA,pan,pan,6,32)
	LOOKUP_IF_BLK(skein224,SKEIN224,skn224,skn224,8,28) LOOKUP_IF_BLK(skein256,SKEIN256,skn256,skn256,8,32)
	LOOKUP_IF_BLK(skein384,SKEIN384,skn384,skn384,8,48) LOOKUP_IF_BLK(skein512,SKEIN512,skn512,skn512,8,64)

	LastTokIsFunc = 0;
	if (!strncmp(pInput, "pad16($p)", 9))   return comp_push_sym("pad16", dynamic_pad16, pInput+9, 0);
	if (!strncmp(pInput, "pad20($p)", 9))   return comp_push_sym("pad20", dynamic_pad20, pInput+9, 0);
	if (!strncmp(pInput, "pad100($p)", 10))  return comp_push_sym("pad100", dynamic_pad100, pInput+10, 0);
	if (!strncmp(pInput, "lc($u)", 6)) { if (bNeedUC&&bNeedUC!=1) return comp_push_sym("X", fpNull, pInput, 0); bNeedU=bNeedUC=1; return comp_push_sym("u_lc", fpNull, pInput+6, 0); }
	if (!strncmp(pInput, "uc($u)", 6)) { if (bNeedUC&&bNeedUC!=2) return comp_push_sym("X", fpNull, pInput, 0); bNeedU=bNeedUC=2; return comp_push_sym("u_uc", fpNull, pInput+6, 0); }
	if (!strncmp(pInput, "lc($p)", 6)) { if (bNeedPC&&bNeedPC!=1) return comp_push_sym("X", fpNull, pInput, 0); bNeedPC=1; return comp_push_sym("p_lc", fpNull, pInput+6, 0); }
	if (!strncmp(pInput, "uc($p)", 6)) { if (bNeedPC&&bNeedPC!=2) return comp_push_sym("X", fpNull, pInput, 0); bNeedPC=2; return comp_push_sym("p_uc", fpNull, pInput+6, 0); }
	LastTokIsFunc = 2;
	//if (!strncmp(pInput, "utf16be", 7)) return comp_push_sym("futf16be", dynamic_futf16be, pInput+7);
	if (!strncmp(pInput, "utf16(", 6))   return comp_push_sym("futf16", dynamic_futf16, pInput+5, 0);
	LastTokIsFunc = 0;
	return comp_push_sym("X", fpNull, pInput, 0);
}

static void comp_lexi_error(DC_struct *p, const char *pInput, char *msg) {
	int n;
	fprintf(stderr, "Dyna expression syntax error around this part of expression\n");
	fprintf(stderr, "%s\n", p->pExpr);
	n = strlen(p->pExpr)-strlen(pInput);
	if (SymTab[nSyms-1][0] != 'X') n--;
	while (n--) fprintf(stderr, " ");
	fprintf(stderr, "^\n");
	if (SymTab[nSyms-1][0] != 'X') fprintf(stderr, "Invalid token found\n");
	else fprintf(stderr, "%s\n", msg);
	error("exiting now");
}
static char *comp_optimize_expression(const char *pExpr) {
	char *pBuf = (char*)mem_alloc(strlen(pExpr)+1), *p, *p2;
	int n1, n2;
	strcpy(pBuf, pExpr);

	/*
	 * Look for crypt($s) optimziation. At this time, we do not look for SALT_AS_HEX_TO_SALT2 variant.
	 */
	p = strstr(pBuf, "($s)");
	n1 = 0;
	while (p) {
		++n1;
		p = strstr(&p[1], "($s)");
	}
	if (n1) {
		// make sure they are all the same crypt type
		char cpType[48];
		p = strstr(pBuf, "($s)");
		--p;
		while (*p != '.' && *p != '(')
			--p;
		p2 = cpType;
		++p;
		while (p[-1] != ')' && p2-cpType < sizeof(cpType)-1)
			*p2++ = *p++;
		*p2 = 0;
		p = strstr(pBuf, cpType);
		n2 = 0;
		while (p) {
			++n2;
			p = strstr(&p[1], cpType);
		}
		if (n1 == n2) {
			// ok, all were same hash type.  Now make sure all $s are in crypt($s)
			n2 = 0;
			p = strstr(pBuf, "$s");
			while (p) {
				++n2;
				p = strstr(&p[1], cpType);
			}
			if (n1 == n2) {
				// we can use SALT_AS_HEX
				salt_as_hex_type = mem_alloc(strlen(cpType)+1);
				strcpy(salt_as_hex_type, cpType);
				p = strstr(pBuf, cpType);
				while (p) {
					memcpy(p, "$s", 2);
					memmove(&p[2], &p[strlen(cpType)], strlen(&p[strlen(cpType)])+1);
					p = strstr(p, cpType);
				}

			} else {
				// we could use MGF_SALT_AS_HEX_TO_SALT2
			}
		}
	}
	/*
	 * End of SALT_AS_HEX optimization
	 */

	/*
	 * Look for common sub-expressions  we handle crypt($p), crypt($s.$p) crypt($p.$s)
	 */
	return pBuf;
}
static int comp_do_lexi(DC_struct *p, const char *pInput) {
	int paren = 0;
	pInput = comp_get_symbol(pInput);
	if (LastTokIsFunc != 1)
		error("Error: dynamic hash must start with md4/md5/sha1 and NOT a *_raw version. This expression one does not\n");
	while (SymTab[nSyms-1][0] != 'X') {
		if (LastTokIsFunc) {
			pInput = comp_get_symbol(pInput);
			if (SymTab[nSyms-1][0] != '(')
				comp_lexi_error(p, pInput, "A ( MUST follow one of the hash function names");
			continue;
		}
		if (SymTab[nSyms-1][0] == '(') {
			pInput = comp_get_symbol(pInput);
			if (SymTab[nSyms-1][0] == 'X' || SymTab[nSyms-1][0] == '.' || SymTab[nSyms-1][0] == '(' || SymTab[nSyms-1][0] == ')' || SymTab[nSyms-1][0] == '^')
				comp_lexi_error(p, pInput, "Invalid token following a ( character");
			++paren;
			continue;
		}
		if (SymTab[nSyms-1][0] == ')') {
			--paren;
			if (*pInput == 0 && *pInput != '^') {
				if (!paren) {
					// expression is VALID and syntax check successful
#ifdef WITH_MAIN
					if (!GEN_BIG)
					printf ("The expression checks out as valid\n");
#endif
					return nSyms;
				}
				comp_lexi_error(p, pInput, "Not enough ) characters at end of expression");
			}
			if (paren == 0 && *pInput != '^')
				comp_lexi_error(p, pInput, "Reached the matching ) to the initial ( and there is still more expression left");
			pInput = comp_get_symbol(pInput);
			// only . ) or ^ are valid to follow a )
			if (!(SymTab[nSyms-1][0] == '.' || SymTab[nSyms-1][0] == ')' || SymTab[nSyms-1][0] == '^'))
				comp_lexi_error(p, pInput, "The only things valid to follow a ) char are a . or a )");
			continue;
		}
		if (SymTab[nSyms-1][0] == '^') {
			if (*pInput == 0) {
				if (!paren) {
					// expression is VALID and syntax check successful
#ifdef WITH_MAIN
					if (!GEN_BIG)
						printf ("The expression checks out as valid\n");
#endif
					return nSyms;
				}
				comp_lexi_error(p, pInput, "Not enough ) characters at end of expression");
			}
			pInput = comp_get_symbol(pInput);
			// only a . or ) are valid
			if (SymTab[nSyms-1][0] != '.' && SymTab[nSyms-1][0] != ')')
				comp_lexi_error(p, pInput, "The only things valid to follow a ^# is a . )");
			continue;
		}
		if (SymTab[nSyms-1][0] == '.') {
			pInput = comp_get_symbol(pInput);
			// any unknown, or a: . ( ) ^  are not valid to follow a .
			if (SymTab[nSyms-1][0] == 'X' || SymTab[nSyms-1][0] == '.' || SymTab[nSyms-1][0] == '(' || SymTab[nSyms-1][0] == ')' || SymTab[nSyms-1][0] == '^')
				comp_lexi_error(p, pInput, "Invalid token following the . character");
			continue;
		}
		// some string op
		pInput = comp_get_symbol(pInput);
		// The only thing that can follow a string op is a . or a )
		if (!(SymTab[nSyms-1][0] == '.' || SymTab[nSyms-1][0] == ')'))
			comp_lexi_error(p, pInput, "The only things valid to follow a string type are a . or a )");
	}
	return 0;
}
static void push_pcode(const char *v, fpSYM _fpSym, int len) {
	pCode[nCode] = mem_alloc(strlen(v)+1);
	fpCode[nCode] = _fpSym;
	nLenCode[nCode] = len;
	strcpy(pCode[nCode++], v);
}

static void comp_do_parse(int cur, int curend) {
	char *curTok;
	fpSYM fpcurTok;
	int curTokLen;
	if (SymTab[cur][0] == '(' && SymTab[curend][0] == ')') {++cur; --curend; }
	while (cur <= curend) {
		curTok = SymTab[cur];
		fpcurTok = fpSymTab[cur];
		curTokLen = nSymTabLen[cur];
		if (*curTok == '.') {
			++cur;
			continue;
		}
		if (*curTok == '^') {
			push_pcode(curTok, dynamic_exp, 0);
			curTok = SymTab[++cur];
			continue;
		}
		if (strlen(curTok)>1 && (*curTok == 'f' || *curTok == 'F')) {
			int tail, count = 1;
			// find the closing ')' for this function.
			++cur; // skip the function name.  Now cur should point to the ( symbol
			tail = cur;
			while(count) {
				++tail;
				if (SymTab[tail][0] == '(') ++count;
				if (SymTab[tail][0] == ')') --count;
			}
			// output code
			push_pcode("push", dynamic_push, 0);
			//. recursion
			comp_do_parse(cur, tail);
			cur = tail+1;
			// now output right code to do the crypt;
			push_pcode(curTok, fpcurTok, curTokLen);
			continue;
		}
		++cur;
		switch(*curTok) {
			case 's':
				//if (!strcmp(gen_stype, "tohex")) push_pcode("app_sh");
				//else
					push_pcode("app_sh", dynamic_app_sh, 0);
				bNeedS = 1;
				continue;
			case 'p':
			{
				if (!strcmp(curTok, "p"))
					push_pcode("app_p", dynamic_app_p, 0);
				else if (!strcmp(curTok, "pad16"))
					push_pcode("pad16", dynamic_pad16, 0);
				else if (!strcmp(curTok, "pad20"))
					push_pcode("pad20", dynamic_pad20, 0);
				else if (!strcmp(curTok, "pad100"))
					push_pcode("pad100", dynamic_pad100, 0);
				else if (!strcmp(curTok, "p_lc")) {
					bNeedPlc = 1;
					push_pcode("app_p_lc", dynamic_app_p_lc, 0);
				} else if (!strcmp(curTok, "p_uc")) {
					bNeedPuc = 1;
					push_pcode("app_p_uc", dynamic_app_p_uc, 0);
				}
				continue;
			}
			case 'S': push_pcode("app_s2", dynamic_app_S, 0); bNeedS2 = 1; continue;
			case 'u':
			{
				bNeedU = 1;
				if (!strcmp(curTok, "u"))
					push_pcode("app_u", dynamic_app_u, 0);
				else if (!strcmp(curTok, "u_lc")) {
					bNeedUlc = 1;
					push_pcode("app_u_lc", dynamic_app_u_lc, 0);
				} else if (!strcmp(curTok, "u_uc")) {
					bNeedUuc = 1;
					push_pcode("app_u_uc", dynamic_app_u_uc, 0);
				}
				continue;
			}
			case '1': push_pcode("app_1", dynamic_app_1, 0); continue;
			case '2': push_pcode("app_2", dynamic_app_2, 0); continue;
			case '3': push_pcode("app_3", dynamic_app_3, 0); continue;
			case '4': push_pcode("app_4", dynamic_app_4, 0); continue;
			case '5': push_pcode("app_5", dynamic_app_5, 0); continue;
			case '6': push_pcode("app_6", dynamic_app_6, 0); continue;
			case '7': push_pcode("app_7", dynamic_app_7, 0); continue;
			case '8': push_pcode("app_8", dynamic_app_8, 0); continue;
		}
	}
}

static void comp_add_script_line(const char *fmt, ...) {
	//static char *pScriptLines[1024];
	//static int nScriptLines;
	va_list va;
	int len, len2;

	len = strlen(fmt)*2;
	pScriptLines[nScriptLines] = mem_alloc(len+1);
	va_start(va, fmt);
	len2 = vsnprintf(pScriptLines[nScriptLines], len, fmt, va);
#ifdef _MSC_VER  // we should find out about MinGW here!!
	pScriptLines[nScriptLines][len] = 0;
	while (len2 == -1) {
		MEM_FREE(pScriptLines[nScriptLines]);
		len *= 2;
		pScriptLines[nScriptLines] = mem_alloc(len+1);
		va_end(va);
		va_start(va, fmt);
		len2 = vsnprintf(pScriptLines[nScriptLines], len, fmt, va);
		pScriptLines[nScriptLines][len] = 0;
	}
#else
	if (len2 > len) {
		MEM_FREE(pScriptLines[nScriptLines]);
		len = len2+1;
		pScriptLines[nScriptLines] = mem_alloc(len+1);
		va_end(va);
		va_start(va, fmt);
		vsnprintf(pScriptLines[nScriptLines], len, fmt, va);
	}
#endif
	va_end(va);
	++nScriptLines;
}

static char *rand_str(int len) {
	static char tmp[256];
	const char *alpha = "0123456789abcdef";
	char *cp = tmp;
	int i;
	if (len > 255)
		len = 255;
	for (i = 0; i < len; ++i) {
		int j = rand() % 16;
		*cp++ = alpha[j];
	}
	*cp = 0;
	return tmp;
}
// Ported from pass_gen.pl dynamic_run_compiled_pcode() function.
static void build_test_string(DC_struct *p, char **pLine) {
	int i;
	char salt[260];
	dynamic_push();
	*gen_s = 0;
	strcpy(gen_plc, gen_pw);
	strcpy(gen_puc, gen_pw);
	strlwr(gen_plc);
	strupr(gen_puc);
	if (bNeedS) {
		if (nSaltLen > 0)
			strcpy(gen_s, rand_str(nSaltLen));
		else
			strcpy(gen_s, rand_str(8));
	}
	if (bNeedU) {
		strcpy(gen_u, rand_str(8));
		strcpy(gen_ulc, gen_u);
		strlwr(gen_ulc);
		strcpy(gen_uuc, gen_u);
		strupr(gen_uuc);
	}
	if (bNeedS2) {
		strcpy(gen_s2, rand_str(8));
	}
	strcpy(salt, gen_s);
	if (salt_as_hex_type) {
		char tmp[64], *cp;
		strcpy(tmp, salt_as_hex_type);
		cp = strchr(tmp, '(');
		*cp = 0;
		strupr(tmp);
		h = gen_s;
		h_len = strlen(h);
#undef IF
#define IF(T,F) if (!strcmp(tmp, #T)) F##_hex()
#undef ELSEIF
#define ELSEIF(T,F) else if (!strcmp(tmp, #T)) F##_hex()

		IF(MD5,md5); ELSEIF(MD4,md4); ELSEIF(SHA1,sha1); ELSEIF(SHA224,sha224); ELSEIF(SHA256,sha256); ELSEIF(SHA384,sha384); ELSEIF(SHA512,sha512);
		ELSEIF(WHIRLPOOL,whirlpool); ELSEIF(TIGER,tiger); ELSEIF(GOST,gost);
		ELSEIF(RIPEMD128,ripemd128); ELSEIF(RIPEMD160,ripemd160); ELSEIF(RIPEMD256,ripemd256); ELSEIF(RIPEMD320,ripemd320);
		ELSEIF(HAVAL128_3,haval128_3); ELSEIF(HAVAL128_4,haval128_4); ELSEIF(HAVAL128_5,haval128_5);
		ELSEIF(HAVAL160_3,haval160_3); ELSEIF(HAVAL160_4,haval160_4); ELSEIF(HAVAL160_5,haval160_5);
		ELSEIF(HAVAL192_3,haval192_3); ELSEIF(HAVAL192_4,haval192_4); ELSEIF(HAVAL192_5,haval192_5);
		ELSEIF(HAVAL224_3,haval224_3); ELSEIF(HAVAL224_4,haval224_4); ELSEIF(HAVAL224_5,haval224_5);
		ELSEIF(HAVAL256_3,haval256_3); ELSEIF(HAVAL256_4,haval256_4); ELSEIF(HAVAL256_5,haval256_5);
		ELSEIF(MD2,md2); ELSEIF(PANAMA,panama);
		ELSEIF(SKEIN224,skein224); ELSEIF(SKEIN256,skein256);
		ELSEIF(SKEIN384,skein384); ELSEIF(SKEIN512,skein512);

		else { error("ERROR in dyna-parser. Have salt_as_hex_type set, but do not KNOW this type of hash\n"); }
	}
	for (nCurCode = 0; nCurCode < nCode; ++nCurCode)
		fpCode[nCurCode]();
	//my $ret = "";
	//if ($gen_needu == 1) { $ret .= "\$dynamic_$gen_num\$$h"; }
	//else { $ret .= "\$dynamic_$gen_num\$$h"; }
	//if ($gen_needs > 0) { $ret .= "\$$gen_soutput"; }
	//if ($gen_needs2 > 0) { if (!defined($gen_stype) || $gen_stype ne "toS2hex") {$ret .= "\$\$2$gen_s2";} }
	//return $ret;
	MEM_FREE(*pLine);
	*pLine = mem_alloc_tiny(strlen(p->pExpr)+strlen(salt)+strlen(gen_Stack[0])+24, 1);
	sprintf(*pLine, "@dynamic=%s@%s", p->pExpr,  gen_Stack[0]);
	if (bNeedS) {
		strcat(*pLine, "$");
		strcat(*pLine, salt);
	}
	if (bNeedU) {
		strcat(*pLine, "$$U");
		strcat(*pLine, gen_u);
	}
	if (bNeedS2) {
		strcat(*pLine, "$$2");
		strcat(*pLine, gen_s2);
	}
	if (bNeedPuc) strupr(gen_pw);
	else if (bNeedPlc) strlwr(gen_pw);
	comp_add_script_line("Test=%s:%s\n", *pLine, gen_pw);
	for (i = 0; i < ngen_Stack_max; ++i)
		MEM_FREE(gen_Stack[i]);
	ngen_Stack = ngen_Stack_max = 0;
}

int b64_len (int rawlen) {
	// this is the formula for mime with trailing = values. It always jumps up
	// an even 4 bytes, when we cross a base64 block threashold. Even though
	// this may return a slightly larger string size than the actual string
	// we put into the buffer, this function is safe. It may simply make
	// MAX_PLAINTEXT_LENGTH a byte or 2 shorter than it possibly could be.
	return (((((rawlen+2)/3)*4)+3)/4)*4;
}
static int parse_expression(DC_struct *p) {
	int i, len;
	char *pExpr, *pScr;
	int salt_hex_len = 0;
	init_static_data();
	// first handle the extra strings
	if (handle_extra_params(p))
		return 1;
	pExpr = comp_optimize_expression(p->pExpr);
	if (!comp_do_lexi(p, pExpr))
		return 1;
	comp_do_parse(0, nSyms-1);
	MEM_FREE(pExpr);

	// Ok, now 'build' the script
	comp_add_script_line("Expression=dynamic=%s\n", p->pExpr);
	comp_add_script_line("Flag=MGF_FLAT_BUFFERS\n");
	if (salt_as_hex_type) {
		char tmp[64], *cp;
		strcpy(tmp, salt_as_hex_type);
		cp = strchr(tmp, '(');
		*cp = 0;
		strupr(tmp);
		comp_add_script_line("Flag=MGF_SALT_AS_HEX_%s\n", tmp);
		if (!strcmp(tmp,"MD5")||!strcmp(tmp,"MD4")||strcmp(tmp,"RIPEMD128")||!strncmp(tmp,"HAVAL_128", 9)||!strcmp(tmp,"MD2")) salt_hex_len = 32;
		if (!strcmp(tmp,"SHA1")||!strcmp(tmp,"RIPEMD160")||!strncmp(tmp,"HAVAL_160", 9)) salt_hex_len = 40;
		if (!strcmp(tmp,"TIGER")||!strncmp(tmp,"HAVAL_192", 9)) salt_hex_len = 48;
		if (!strcmp(tmp,"SHA224")||!strncmp(tmp,"HAVAL_224", 9)||!strcmp(tmp,"SKEIN224")) salt_hex_len = 56;
		if (!strcmp(tmp,"SHA256")||!strcmp(tmp,"RIPEMD256")||!strcmp(tmp,"GOST")||!strncmp(tmp,"HAVAL_256",9)||!strcmp(tmp,"PANAMA")||!strcmp(tmp,"SKEIN256")) salt_hex_len = 64;
		if (!strcmp(tmp,"RIPEMD320")) salt_hex_len = 80;
		if (!strcmp(tmp,"SHA384")||!strcmp(tmp,"SKEIN384")) salt_hex_len = 96;
		if (!strcmp(tmp,"SHA512")||!strcmp(tmp,"WHIRLPOOL")||!strcmp(tmp,"SKEIN512")) salt_hex_len = 128;
	}
	if (bNeedS) comp_add_script_line("Flag=MGF_SALTED\n");
	if (bNeedS2) comp_add_script_line("Flag=MGF_SALTED2\n");
	if (bNeedPuc)
		comp_add_script_line("Flag=MGF_PASSWORD_UPCASE\n");
	if (bNeedPlc)
		comp_add_script_line("Flag=MGF_PASSWORD_LOCASE\n");
	if (bNeedU) {
		if (bNeedUuc)
			comp_add_script_line("Flag=MGF_USERNAME_UPCASE\n");
		else if (bNeedUlc)
			comp_add_script_line("Flag=MGF_USERNAME_LOCASE\n");
		else
			comp_add_script_line("Flag=MGF_USERNAME\n");
	}
	for (i = 1; i < 9; ++i) {
		if (Const[i]) {
			comp_add_script_line("Const%d=%s\n", i, Const[i]);
		} else
			break;
	}

	if (compile_debug) {
		for (i = 0; i <nCode; ++i)
			printf ("%s\n", pCode[i]);
	}

	// Build test strings.
	strcpy(gen_pw, "abc");
	build_test_string(p, &p->pLine1);
	strcpy(gen_pw, "john");
	build_test_string(p, &p->pLine2);
	strcpy(gen_pw, "passweird");
	build_test_string(p, &p->pLine3);

	// Ok now run the script
	{
		int x, j, last_push;
		//int inp2_used = 0;
		int salt_len = nSaltLen ? nSaltLen : -32;
		int in_unicode = 0, out_raw = 0, out_64 = 0, out_16u = 0;
		int append_mode = 0;
		int max_inp_len = 110, len_comp = 0;
		int inp1_clean = 0, exponent = -1;
		int use_inp1 = 1, use_inp1_again = 0;
		int inp_cnt = 0, ex_cnt = 0, salt_cnt = 0, hash_cnt = 0, flag_utf16 = 0;

		if (bNeedS) {
			comp_add_script_line("SaltLen=%d\n", salt_len);
			if (salt_hex_len)
				salt_len = salt_hex_len;
		} else
			salt_len = 0;
		if (salt_len < 0)
			salt_len *= -1;
		if (!keys_as_input) {
			comp_add_script_line("Func=DynamicFunc__clean_input_kwik\n");
			inp1_clean = 1;
		}
		for (i = 0; i < nCode; ++i) {
			if (pCode[i][0] == 'f' || pCode[i][0] == 'F') {

				if (!inp1_clean && !keys_as_input) {
					comp_add_script_line("Func=DynamicFunc__clean_input_kwik\n");
					inp1_clean = 1;
				}
				//if (!strcasecmp(pCode[i], "futf16be") || !strcasecmp(pCode[i], "futf16")) {
				if (!strcasecmp(pCode[i], "futf16")) {
					if (!in_unicode) {
						in_unicode = 1;
						comp_add_script_line("Func=DynamicFunc__setmode_unicode\n");
					}
					if (!flag_utf16) {
						comp_add_script_line("Flag=MGF_UTF8\n");
						flag_utf16 = 1;
					}
				} else if ((pCode[i][0] == 'f' || pCode[i][0] == 'F') && pCode[i][strlen(pCode[i])-1] == 'r') {
					if (!out_raw) {
						out_raw = 1;
						comp_add_script_line("Func=DynamicFunc__LargeHash_OUTMode_raw\n");
					}
				} else if ((pCode[i][0] == 'f' || pCode[i][0] == 'F') && pCode[i][strlen(pCode[i])-1] == 'H') {
					if (!out_16u) {
						out_16u = 1;
						comp_add_script_line("Func=DynamicFunc__LargeHash_OUTMode_base16u\n");
					}
				} else if ((pCode[i][0] == 'f' || pCode[i][0] == 'F') && pCode[i][strlen(pCode[i])-1] == '6') {
					if (!out_64) {
						out_64 = 1;
						comp_add_script_line("Func=DynamicFunc__LargeHash_OUTMode_base64\n");
					}
				} else {
					// if final hash, then dont clear the mode to normal
					if ( in_unicode && !(!pCode[i+1] || !pCode[i+1][0]))
						comp_add_script_line("Func=DynamicFunc__setmode_normal\n");
					in_unicode = 0;
					if ( (out_raw||out_64||out_16u) && !(!pCode[i+1] || !pCode[i+1][0]))
						comp_add_script_line("Func=DynamicFunc__LargeHash_OUTMode_base16\n");
					out_raw = out_64 = out_16u = 0;
				}
				// Found next function.  Now back up and load the data
				for (j = i - 1; j >= 0; --j) {
					if (!strcmp(pCode[j], "push") || (exponent >= 0 && !strcmp(pCode[j], "Xush"))) { // push
						last_push = j;
						use_inp1_again = 0;
						inp_cnt = 0, ex_cnt = 0, salt_cnt = 0, hash_cnt = 0;
						for (x = j+1; x < i; ++x) {
							if (!strncmp(pCode[x], "app_p", 5)) {
								comp_add_script_line("Func=DynamicFunc__append_keys%s\n", use_inp1?"":"2"); ++inp_cnt; }
							else if (!strcmp(pCode[x], "app_s")) {
								comp_add_script_line("Func=DynamicFunc__append_salt%s\n", use_inp1?"":"2"); ++salt_cnt; }
							else if (!strncmp(pCode[x], "app_u", 5)) {
								comp_add_script_line("Func=DynamicFunc__append_userid%s\n", use_inp1?"":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_s2")) {
								comp_add_script_line("Func=DynamicFunc__append_2nd_salt%s\n", use_inp1?"":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_sh")) {
								comp_add_script_line("Func=DynamicFunc__append_salt%s\n", use_inp1?"":"2"); ++salt_cnt; }
							else if (!strcmp(pCode[x], "app_1")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST1\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_2")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST2\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_3")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST3\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_4")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST4\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_5")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST5\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_6")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST6\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_7")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST7\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strcmp(pCode[x], "app_8")) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_CONST8\n", use_inp1?"1":"2"); ++ex_cnt; }
							else if (!strncmp(pCode[x], "IN2", 3)) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_input2\n", use_inp1?"":"2"); ++hash_cnt; }
							else if (!strncmp(pCode[x], "IN1", 3)) {
								comp_add_script_line("Func=DynamicFunc__append_input%s_from_input\n", use_inp1?"":"2"); ++hash_cnt; }
							else if (!strcmp(pCode[x], "pad16")) {
								comp_add_script_line("Func=DynamicFunc__append_keys_pad16\n"); ++hash_cnt; }
							else if (!strcmp(pCode[x], "pad20")) {
								comp_add_script_line("Func=DynamicFunc__append_keys_pad20\n"); ++hash_cnt; }
							else if (!strcmp(pCode[x], "pad100")) {
								comp_add_script_line("Func=DynamicFunc__set_input_len_100\n"); len_comp += 100; }

							*pCode[x] = 'X';
						}
						if (!last_push || pCode[last_push-1][0] == 'p')
							pCode[last_push][0] = 'X';
						else {
							strcpy(pCode[last_push], "IN2");
							inp1_clean = 0;
							use_inp1_again = 1;
						}

						// Ok, the only thing we can control is salt_len (if not in hex_as_salt), and inp_len
						// all we worry about is inp_len.  256 bytes is MAX.
						len_comp += ex_cnt*24;
						len_comp += inp_cnt*max_inp_len;
						len_comp += salt_cnt*salt_len;
						// add in hash_cnt*whatever_size_hash is.
						if (!strncasecmp(pCode[i], "f512", 4 ) || !strncasecmp(pCode[i], "f384", 4)) {
							// the only 64 bit SIMD hashes we have right now. are sha284 and sha512
							if (len_comp > 239) {
								max_inp_len -= (len_comp-239+(inp_cnt-1))/inp_cnt;
							}
						} else if (!strncasecmp(pCode[i], "f5", 2 ) || !strncasecmp(pCode[i], "f4", 2) ||
							       !strncasecmp(pCode[i], "f1", 2) || !strncasecmp(pCode[i], "f224", 4 ) ||
								   !strncasecmp(pCode[i], "f256", 4)) {
							// the only 32 bit SIMD hashes we have right now. are sha284 and sha512
							if (len_comp > 247) {
								max_inp_len -= (len_comp-247+(inp_cnt-1))/inp_cnt;
							}
						} else {
							// non SIMD code can use full 256 byte buffers.
							if (len_comp > 256) {
								max_inp_len -= (len_comp-256+(inp_cnt-1))/inp_cnt;
							}
						}
						len_comp = 0;
						if (!pCode[i+1] || !pCode[i+1][0]) {
							// final hash
							char endch = pCode[i][strlen(pCode[i])-1];
							if (endch == 'c') {
								comp_add_script_line("Flag=MGF_INPBASE64b\n");
							} else if (endch == '6') {
								comp_add_script_line("Flag=MGF_INPBASE64m\n");
							} else if (endch == 'H') {
								comp_add_script_line("Flag=MGF_BASE_16_OUTPUT_UPCASE\n");
							}
							// check for sha512 has to happen before md5, since both start with f5
#undef IF
#undef ELSEIF
#define IF(C,T,L,F) if (!strncasecmp(pCode[i], #T, L)) { \
	comp_add_script_line("Func=DynamicFunc__" #C "_crypt_input%s_to_output1_FINAL\n", use_inp1?"1":"2"); \
	if(F) { comp_add_script_line("Flag=MGF_INPUT_" #F "_BYTE\n"); outer_hash_len = F; } else outer_hash_len = 16; }
#define ELSEIF(C,T,L,F) else if (!strncasecmp(pCode[i], #T, L)) { \
	comp_add_script_line("Func=DynamicFunc__" #C "_crypt_input%s_to_output1_FINAL\n", use_inp1?"1":"2"); \
	if(F) { comp_add_script_line("Flag=MGF_INPUT_" #F "_BYTE\n"); outer_hash_len = F; } else outer_hash_len = 16; }

							IF(SHA512,f512,4,64)
							ELSEIF(MD5,f5,2,0)
							ELSEIF(MD4,f4,2,0)
							ELSEIF(SHA1,f1,2,20)
							ELSEIF(SHA224,f224,4,28) ELSEIF(SHA256,f256,4,32) ELSEIF(SHA384,f384,4,48) ELSEIF(SHA512,f512,4,64)
							ELSEIF(GOST,fgost,5,32)
							ELSEIF(Tiger,ftig,4,24)
							ELSEIF(WHIRLPOOL,fwrl,4,64)
							ELSEIF(RIPEMD128,frip128,7,0) ELSEIF(RIPEMD160,frip160,7,20) ELSEIF(RIPEMD256,frip256,7,32) ELSEIF(RIPEMD320,frip320,7,40)
							ELSEIF(HAVAL128_3,fhav128_3,9,0)  ELSEIF(HAVAL128_4,fhav128_4,9,0)  ELSEIF(HAVAL128_5,fhav128_5,9,0)
							ELSEIF(HAVAL160_3,fhav160_3,9,20) ELSEIF(HAVAL160_4,fhav160_4,9,20) ELSEIF(HAVAL160_5,fhav160_5,9,20)
							ELSEIF(HAVAL192_3,fhav192_3,9,24) ELSEIF(HAVAL192_4,fhav192_4,9,24) ELSEIF(HAVAL192_5,fhav192_5,9,24)
							ELSEIF(HAVAL224_3,fhav224_3,9,28) ELSEIF(HAVAL224_4,fhav224_4,9,28) ELSEIF(HAVAL224_5,fhav224_5,9,28)
							ELSEIF(HAVAL256_3,fhav256_3,9,32) ELSEIF(HAVAL256_4,fhav256_4,9,32) ELSEIF(HAVAL256_5,fhav256_5,9,32)
							ELSEIF(MD2,fmd2,4,0) ELSEIF(PANAMA,fpan,4,32)
							ELSEIF(SKEIN224,fskn224,7,28) ELSEIF(SKEIN256,fskn256,7,32)
							ELSEIF(SKEIN384,fskn384,7,48) ELSEIF(SKEIN512,fskn512,7,64)
						} else {
							if (append_mode) {
#undef IF
#undef ELSEIF
#define IF(C,T,L) if (!strncasecmp(pCode[i], #T, L)) \
	{ \
		char type = pCode[i][strlen(pCode[i])-1]; \
		comp_add_script_line("Func=DynamicFunc__" #C "_crypt_input%s_append_input2\n", use_inp1?"1":"2"); \
		if (type=='r') { len_comp += nLenCode[i]; } \
		else if (type=='c'||type=='6') { len_comp += b64_len(nLenCode[i]); } \
		else { len_comp += nLenCode[i]*2; } \
	}
#define ELSEIF(C,T,L) else if (!strncasecmp(pCode[i], #T, L)) \
	{ \
		char type = pCode[i][strlen(pCode[i])-1]; \
		comp_add_script_line("Func=DynamicFunc__" #C "_crypt_input%s_append_input2\n", use_inp1?"1":"2"); \
		if (type=='r') { len_comp += nLenCode[i]; } \
		else if (type=='c'||type=='6') { len_comp += b64_len(nLenCode[i]); } \
		else { len_comp += nLenCode[i]*2; } \
	}

								IF(SHA512,f512,4)
								ELSEIF(MD5,f5,2)
								ELSEIF(MD4,f4,2)
								ELSEIF(SHA1,f1,2)
								ELSEIF(SHA224,f224,4) ELSEIF(SHA256,f256,4) ELSEIF(SHA384,f384,4) ELSEIF(SHA512,f512,4)
								ELSEIF(GOST,fgost,5)
								ELSEIF(Tiger,ftig,4)
								ELSEIF(WHIRLPOOL,fwrl,4)
								ELSEIF(RIPEMD128,frip128,7) ELSEIF(RIPEMD160,frip160,7) ELSEIF(RIPEMD256,frip256,7) ELSEIF(RIPEMD320,frip320,7)
								ELSEIF(HAVAL128_3,fhav128_3,9) ELSEIF(HAVAL128_4,fhav128_4,9) ELSEIF(HAVAL128_5,fhav128_5,9)
								ELSEIF(HAVAL160_3,fhav160_3,9) ELSEIF(HAVAL160_4,fhav160_4,9) ELSEIF(HAVAL160_5,fhav160_5,9)
								ELSEIF(HAVAL192_3,fhav192_3,9) ELSEIF(HAVAL192_4,fhav192_4,9) ELSEIF(HAVAL192_5,fhav192_5,9)
								ELSEIF(HAVAL224_3,fhav224_3,9) ELSEIF(HAVAL224_4,fhav224_4,9) ELSEIF(HAVAL224_5,fhav224_5,9)
								ELSEIF(HAVAL256_3,fhav256_3,9) ELSEIF(HAVAL256_4,fhav256_4,9) ELSEIF(HAVAL256_5,fhav256_5,9)
								ELSEIF(MD2,fmd2,4) ELSEIF(PANAMA,fpan,4)
								ELSEIF(SKEIN224,fskn224,7) ELSEIF(SKEIN256,fskn256,7)
								ELSEIF(SKEIN384,fskn384,7) ELSEIF(SKEIN512,fskn512,7)
								else {
									if (use_inp1 && !use_inp1_again)
										use_inp1_again = 1;
								}
						} else { // overwrite mode.
#undef IF
#undef ELSEIF
#define IF(C,T,L) if (!strncasecmp(pCode[i], #T, L)) \
	{ \
		char type = pCode[i][strlen(pCode[i])-1]; \
		comp_add_script_line("Func=DynamicFunc__" #C "_crypt_input%s_overwrite_input2\n", use_inp1?"1":"2"); \
		if (type=='r') { len_comp = nLenCode[i]; } \
		else if (type=='c'||type=='6') { len_comp = b64_len(nLenCode[i]); } \
		else { len_comp = nLenCode[i]*2; } \
	}
#define ELSEIF(C,T,L) else if (!strncasecmp(pCode[i], #T, L)) \
	{ \
		char type = pCode[i][strlen(pCode[i])-1]; \
		comp_add_script_line("Func=DynamicFunc__" #C "_crypt_input%s_overwrite_input2\n", use_inp1?"1":"2"); \
		if (type=='r') { len_comp = nLenCode[i]; } \
		else if (type=='c'||type=='6') { len_comp = b64_len(nLenCode[i]); } \
		else { len_comp = nLenCode[i]*2; } \
	}

								IF(SHA512,f512,4)
								ELSEIF(MD5,f5,2)
								ELSEIF(MD4,f4,2)
								ELSEIF(SHA1,f1,2)
								ELSEIF(SHA224,f224,4) ELSEIF(SHA256,f256,4) ELSEIF(SHA384,f384,4) ELSEIF(SHA512,f512,4)
								ELSEIF(GOST,fgost,5)
								ELSEIF(Tiger,ftig,4)
								ELSEIF(WHIRLPOOL,fwrl,4)
								ELSEIF(RIPEMD128,frip128,7) ELSEIF(RIPEMD160,frip160,7) ELSEIF(RIPEMD256,frip256,7) ELSEIF(RIPEMD320,frip320,7)
								ELSEIF(HAVAL128_3,fhav128_3,9) ELSEIF(HAVAL128_4,fhav128_4,9) ELSEIF(HAVAL128_5,fhav128_5,9)
								ELSEIF(HAVAL160_3,fhav160_3,9) ELSEIF(HAVAL160_4,fhav160_4,9) ELSEIF(HAVAL160_5,fhav160_5,9)
								ELSEIF(HAVAL192_3,fhav192_3,9) ELSEIF(HAVAL192_4,fhav192_4,9) ELSEIF(HAVAL192_5,fhav192_5,9)
								ELSEIF(HAVAL224_3,fhav224_3,9) ELSEIF(HAVAL224_4,fhav224_4,9) ELSEIF(HAVAL224_5,fhav224_5,9)
								ELSEIF(HAVAL256_3,fhav256_3,9) ELSEIF(HAVAL256_4,fhav256_4,9) ELSEIF(HAVAL256_5,fhav256_5,9)
								ELSEIF(MD2,fmd2,4) ELSEIF(PANAMA,fpan,4)
								ELSEIF(SKEIN224,fskn224,7) ELSEIF(SKEIN256,fskn256,7)
								ELSEIF(SKEIN384,fskn384,7) ELSEIF(SKEIN512,fskn512,7)
								else {
									if (use_inp1 && !use_inp1_again)
										use_inp1_again = 1;
								}
							}
							use_inp1 = append_mode = 0;
							if (use_inp1_again)
								use_inp1 = 1;
							if (pCode[i+1] && pCode[i+1][0] == 'p') {
								inp1_clean = 0;
								append_mode = 1;
								use_inp1 = 1;
							}
						}
						break;
					}
				}
				if (i==nCode-1 || pCode[i+1][0] != '^')
					pCode[i][0] = 'X';
				else {
					if (exponent < 1)
						exponent = atoi(&pCode[i+1][1])-2;
					else
						--exponent;
					if (!exponent) {
						int k;
						MEM_FREE(pCode[i+1]);
						for (k = i+1; k < nCode; ++k) {
							pCode[k] = pCode[k+1];
							fpCode[k] = fpCode[k+1];
						}
						--nCode;
					}
					--i;
				}
			}
		}
		comp_add_script_line("MaxInputLenX86=%d\n",max_inp_len);
		comp_add_script_line("MaxInputLen=%d\n",max_inp_len);
	}

	len = i = 0;
	for (i = 0; i < nScriptLines; ++i)
		len += strlen(pScriptLines[i]);
	pScr = mem_alloc_tiny(len+1,1);
	*pScr = 0;
	for (i = 0; i < nScriptLines; ++i)
		strcat(pScr, pScriptLines[i]);
	p->pScript = pScr;

	if (compile_debug) {
		printf("%s\n", p->pScript);
		exit(0);
	}

	return 0;
}

static DC_HANDLE do_compile(const char *expr, uint32_t crc32) {
	DC_struct *p;
	char *cp;
	const char *cp2;
	int len;

	if (!gost_init) {
		gost_init_table();
		gost_init = 1;
	}

	p = mem_calloc_tiny(sizeof(DC_struct), sizeof(void*));
	p->magic = ~DC_MAGIC;
	if (strncmp(expr, "dynamic=", 8))
		return p;
	p->crc32 = crc32;
	p->pFmt = NULL; // not setup yet
	p->pExpr = str_alloc_copy(find_the_expression(expr));
	p->pExtraParams = str_alloc_copy(find_the_extra_params(expr));
	len = strlen(expr)+3;
	// start of hexify code for : convert into \x3a
	cp = strchr(expr, ':');
	if (cp)
		do {
			cp = strchr(&cp[1], ':');
			len += 3;	// \x3a is 3 bytes longer than :
		} while (cp);
	cp = mem_calloc_tiny(len, 1);
	p->pSignature = cp;
	//snprintf(cp, len, "@%s@", expr); /* switch ':' into \x3a in result */
	cp2 = expr;
	*cp++ = '@';
	while (*cp2) {
		if (*cp2 == ':') {
			strcpy(cp, "\\x3a"); // copy the literal string \x3a to the output
			++cp2;
			cp += 4;
		} else
			*cp++ = *cp2++;
	}
	*cp++ = '@';
	*cp = 0;
	if (parse_expression(p)) {
		return p;
	}
	p->magic = DC_MAGIC;

	return p;
}

static uint32_t compute_checksum(const char *expr) {
	uint32_t crc32 = 0xffffffff;
	/* we should 'normalize' the expression 'first' */
	while (*expr) {
		crc32 = jtr_crc32(crc32,*expr);
		++expr;
	}
	return crc32;
}

static DC_HANDLE find_checksum(uint32_t crc32) {
//	DC_list *p;
	if (!pList)
		pList = mem_calloc_tiny(sizeof(DC_list), sizeof(void*));
	//p = pList->next;
	//while (p) {
	//	if (p->value->crc32 == crc32)
	//		return p->value;
	//	p = p->next;
	//}
	return 0;
}

static void add_checksum_list(DC_HANDLE pHand) {
	DC_list *p;
	p = mem_calloc_tiny(sizeof(DC_list), sizeof(void*));
	p->next = pList->next;
	pList->next = p;
}

static char *convert_old_dyna_to_new(char *fld0, char *in, char *out, int outsize, char *expr) {
	char *cp = strchr(&in[1], '$');
	if (!cp)
		return in;
	++cp;
	snprintf(out, outsize-1, "@dynamic=%s@%s", expr, cp);
	out[outsize-1] = 0;
	if (strstr(expr, "$u") && !strstr(out, "$$U")) {
		strcat (out, "$$U");
		strcat (out, fld0);
	}
	return out;
}

int looks_like_bare_hash(const char *fld1) {
	// look for hex string with 'optional' '$' for salt.
	int len = base64_valid_length(fld1, e_b64_hex, 0);
	if (len == (outer_hash_len<<1)) {
		// check salt flag
		return 1;
	}
	return 0;
}

char *dynamic_compile_prepare(char *fld0, char *fld1) {
	static char Buf[1024], tmp1[64];
	char *cpExpr=0;
	if (!strncmp(fld1, "$dynamic_", 9)) {
		int num;
		if (strlen(fld1) > 490)
			return fld1;
		if (strstr(fld1, "$HEX$")) {
			char *cpBuilding=fld1;
			char *cp, *cpo;
			int bGood=1;
			static char ct[512];

			strcpy(ct, cpBuilding);
			cp = strstr(ct, "$HEX$");
			cpo = cp;
			*cpo++ = *cp;
			cp += 5;
			while (*cp && bGood) {
				if (*cp == '0' && cp[1] == '0') {
					bGood = 0;
					break;
				}
				if (atoi16[ARCH_INDEX(*cp)] != 0x7f && atoi16[ARCH_INDEX(cp[1])] != 0x7f) {
					*cpo++ = atoi16[ARCH_INDEX(*cp)]*16 + atoi16[ARCH_INDEX(cp[1])];
					*cpo = 0;
					cp += 2;
				} else if (*cp == '$') {
					while (*cp && strncmp(cp, "$HEX$", 5)) {
						*cpo++ = *cp++;
					}
					*cpo = 0;
					if (!strncmp(cp, "$HEX$", 5)) {
						*cpo++ = *cp;
						cp += 5;
					}
				} else {
					return fld1;
				}
			}
			if (bGood)
				cpBuilding = ct;
			// if we came into $HEX$ removal, then cpBuilding will always be shorter
			fld1 = cpBuilding;
		}
		if (sscanf(fld1, "$dynamic_%d$", &num) == 1) {
			if (num >= 50 && num < 1000) {
				char *type = 0;
				switch (num/10) {
					case 5: type="sha224"; break; // 50-59
					case 6: type="sha256"; break; // 60-69, etc
					case 7: type="sha384"; break;
					case 8: type="sha512"; break;
					case 9: type="gost"; break;
					case 10: type="whirlpool"; break;
					case 11: type="tiger"; break;
					case 12: type="ripemd128"; break;
					case 13: type="ripemd160"; break;
					case 14: type="ripemd256"; break;
					case 15: type="ripemd320"; break;
					case 16: type="haval128_3"; break;
					case 17: type="haval128_4"; break;
					case 18: type="haval128_5"; break;
					case 19: type="haval160_3"; break;
					case 20: type="haval160_4"; break;
					case 21: type="haval160_5"; break;
					case 22: type="haval192_3"; break;
					case 23: type="haval192_4"; break;
					case 24: type="haval192_5"; break;
					case 25: type="haval224_3"; break;
					case 26: type="haval224_4"; break;
					case 27: type="haval224_5"; break;
					case 28: type="haval256_3"; break;
					case 29: type="haval256_4"; break;
					case 30: type="haval256_5"; break;
					case 31: type="md2"; break;
					case 32: type="panama"; break;
					case 33: type="skein224"; break;
					case 34: type="skein256"; break;
					case 35: type="skein384"; break;
					case 36: type="skein512"; break;
				}
				if (type) {
					switch(num%10) {
						case 0: sprintf(tmp1, "%s($p)", type); break;
						case 1: sprintf(tmp1, "%s($s.$p)", type); break;
						case 2: sprintf(tmp1, "%s($p.$s)", type); break;
						case 3: sprintf(tmp1, "%s(%s($p))", type, type); break;
						case 4: sprintf(tmp1, "%s(%s_raw($p))", type, type); break;
						case 5: sprintf(tmp1, "%s(%s($p).$s)", type, type); break;
						case 6: sprintf(tmp1, "%s($s.%s($p))", type, type); break;
						case 7: sprintf(tmp1, "%s(%s($s).%s($p))", type, type, type); break;
						case 8: sprintf(tmp1, "%s(%s($p).%s($p))", type, type, type); break;
					}
					cpExpr = tmp1;
				}
			} else
			switch(num) {
				case 0: cpExpr = "md5($p)"; break;
				case 1: cpExpr = "md5($p.$s)"; break;
				case 2: cpExpr = "md5(md5($p))"; break;
				case 3: cpExpr = "md5(md5(md5($p)))"; break;
				case 4: cpExpr = "md5($s.$p)"; break;
				case 5: cpExpr = "md5($s.$p.$s)"; break;
				case 6: cpExpr = "md5(md5($p).$s)"; break;
				case 8: cpExpr = "md5(md5($s).$p)"; break;
				case 9: cpExpr = "md5($s.md5($p))"; break;
				case 10: cpExpr = "md5($s.md5($s.$p))"; break;
				case 11: cpExpr = "md5($s.md5($p.$s))"; break;
				case 12: cpExpr = "md5(md5($s).md5($p))"; break;
				case 13: cpExpr = "md5(md5($p).md5($s))"; break;
				case 14: cpExpr = "md5($s.md5($p).$s)"; break;
				case 15: cpExpr = "md5($u.md5($p).$s)"; break;
				case 16: cpExpr = "md5(md5(md5($p).$s).$s2)"; break;
				case 22: cpExpr = "md5(sha1($p))"; break;
				case 23: cpExpr = "sha1(md5($p))"; break;
				case 24: cpExpr = "sha1($p.$s)"; break;
				case 25: cpExpr = "sha1($s.$p)"; break;
				case 26: cpExpr = "sha1($p)"; break;
				case 29: cpExpr = "md5(utf16($p))"; break;
				case 30: cpExpr = "md4($p)"; break;
				case 31: cpExpr = "md4($s.$p)"; break;
				case 32: cpExpr = "md4($p.$s)"; break;
				case 33: cpExpr = "md4(utf16($p))"; break;
				case 34: cpExpr = "md5(md4($p))"; break;
				case 35: cpExpr = "sha1(uc($u).$c1.$p),c1=\\x3a"; break;
				case 36: cpExpr = "sha1($u.$c1.$p),c1=\\x3a"; break;
				case 37: cpExpr = "sha1(lc($u).$p)"; break;
				case 38: cpExpr = "sha1($s.sha1($s.sha1($p)))"; break;
				case 39: cpExpr = "md5($s.pad16($p)),saltlen=-120"; break;
				case 40: cpExpr = "sha1($s.pad20($p)),saltlen=-120"; break;
				//case 30: cpExpr = ""; break;
				//case 30: cpExpr = ""; break;
				//case 30: cpExpr = ""; break;
			}
			if (cpExpr)
				fld1 = convert_old_dyna_to_new(fld0, fld1, Buf, sizeof(Buf), cpExpr);
		}
	}
	// NOTE, we probably could have used dyna_signature at this point, vs creating this options_format item.
	else if (strstr(options_format, "$u") && fld0 && *fld0 && !strstr(fld1, "$$U")) {
		char buf2[sizeof(Buf)];
		// note that Buf may already be fld1, so do not printf into Buf from here on!!
		if (*fld1 == '@')
			snprintf(buf2, sizeof(buf2), "%s$$U%s", fld1, fld0);
		else
			snprintf(buf2, sizeof(buf2), "@%s@%s$$U%s", options_format, fld1, fld0);
		strcpy(Buf, buf2);
		fld1 = Buf;
	} else if (strncmp(fld1, "@dynamic=", 9) && looks_like_bare_hash(fld1)) {
		char buf2[sizeof(Buf)];
		snprintf(buf2, sizeof(buf2), "%s%s", dyna_signature, fld1);
		strcpy(Buf, buf2);
		fld1 = Buf;
	}
	return dynamic_expr_normalize(fld1);
}
char *dynamic_compile_split(char *ct) {
	extern int ldr_in_pot;
	if (strncmp(ct, "dynamic_", 8)) {
		return dynamic_compile_prepare("", ct);
	} else if (strncmp(ct, "@dynamic=", 9) && strncmp(ct, dyna_signature, dyna_sig_len)) {
		// convert back into dynamic= format
		// Note we should probably ONLY do this on 'raw' hashes!
		static char Buf[512];
		sprintf(Buf, "%s%s", dyna_signature, ct);
		ct = Buf;
	} else {
		if (ldr_in_pot == 1 && !strncmp(ct, "@dynamic=", 9)) {
			static char Buf[512], Buf2[512];
			char *cp = strchr(&ct[1], '@');
			if (cp) {
				strcpy(Buf, &cp[1]);
				sprintf(Buf2, "%s%s", dyna_signature, Buf);
				ct = Buf2;
			}
		}
	}
	// ok, now 'normalize' the hash if it is dynamic= hash.
	ct = dynamic_expr_normalize(ct);
	return ct;
}

int dynamic_assign_script_to_format(DC_HANDLE H, struct fmt_main *pFmt) {
	if (!((DC_struct*)H) || ((DC_struct*)H)->magic != DC_MAGIC)
		return -1;
	dyna_script = ((DC_struct*)H)->pScript;
	dyna_signature = ((DC_struct*)H)->pSignature;
	dyna_line1 = ((DC_struct*)H)->pLine1;
	dyna_line2 = ((DC_struct*)H)->pLine2;
	dyna_line3 = ((DC_struct*)H)->pLine3;
	dyna_sig_len = strlen(dyna_signature);
	((DC_struct*)H)->pFmt = pFmt;
	return 0;
}
void dynamic_compile_done() {
	init_static_data(); /* this will free all allocated crap */
}
#ifdef WITH_MAIN
int ldr_in_pot = 0;

/*****************************************************************************
 * these functions were missing from dynamic_utils.c, so I simply add dummy
 * functions here.  I needed to access dynamic_Demangle() from that file,
 * but there was other baggage along for the ride. When built with WITH_MAIN
 * we use no other code from dynamic_utils.c, so these stubs are safe.
 ****************************************************************************/
int dynamic_IS_VALID(int i, int force) {return 0;}
char *dynamic_LOAD_PARSER_SIGNATURE(int which) {return 0;}
void cfg_init(char *name, int allow_missing) {}
int cfg_get_bool(char *section, char *subsection, char *param, int def) {return 0;}
char *dynamic_PRELOAD_SIGNATURE(int cnt) {return 0;}
/* END of missing functions from dynamic_utils.c */

int big_gen_one(int Num, char *cpExpr) {
	DC_HANDLE p;
	DC_struct *p2;
	int ret;

	ret = dynamic_compile(cpExpr, &p);
	p2 = (DC_struct *)p;
	if (ret || !p2->pScript) return !!printf ("Error, null script variable in type %d\n", Num);
	printf ("static struct fmt_tests _Preloads_%d[] = {\n", Num);
	printf ("    {\"$dynamic_%d$%s\",\"abc\"},\n",Num, strchr(&(p2->pLine1[1]), '@')+1);
	printf ("    {\"$dynamic_%d$%s\",\"john\"},\n",Num, strchr(&(p2->pLine2[1]), '@')+1);
	printf ("    {\"$dynamic_%d$%s\",\"passweird\"},\n",Num, strchr(&(p2->pLine3[1]), '@')+1);
	printf ("    {NULL}};\n");
	return 0;
}
int big_gen(char *cpType, char *cpNum) {
	int Num = atoi(cpNum)*10, ret;
	char szExpr[128];
	char cpTypeU[64];
	DC_HANDLE p;

	GEN_BIG=1;
	strcpy(cpTypeU, cpType);
	strupr(cpTypeU);

	sprintf(szExpr, "dynamic=%s($p)", cpType); //160
	ret = dynamic_compile(szExpr, &p);
	if (ret) return 1;

	printf ("/*** Large hash group for %s dynamic_%d to dynamic_%d ***/\n", cpType, Num, Num+8);
	printf ("DYNA_PRE_DEFINE_LARGE_HASH(%s,%s,%d)\n", cpTypeU, cpNum, (int)strlen(gen_conv)); // gen_conv still holds the last hash from the simple hash($p) expression.

	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s($s.$p)", cpType); //161
	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s($p.$s)", cpType); //162
	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s(%s($p))", cpType, cpType); //163
	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s(%s_raw($p))", cpType, cpType); //164
	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s(%s($p).$s)", cpType, cpType); //165
	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s($s.%s($p))", cpType, cpType); //166
	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s(%s($s).%s($p))", cpType, cpType, cpType); //167
	if (big_gen_one(Num++, szExpr)) return 1;
	sprintf(szExpr, "dynamic=%s(%s($p).%s($p))", cpType, cpType, cpType); //168
	if (big_gen_one(Num++, szExpr)) return 1;

	return 0;
}
int main(int argc, char **argv) {
	DC_HANDLE p;
	DC_struct *p2;
	int ret;

	CRC32_Init_tab();
	compile_debug = 1;
	if (argc == 4 && !strcmp(argv[1], "BIG_GEN"))
		return big_gen(argv[2], argv[3]);
	printf("processing this expression: %s\n\n", argv[1]);
	ret = dynamic_compile(argv[1], &p);
	p2 = (DC_struct *)p;
	if (ret || !p2->pScript) return !!printf ("Error, null script variable\n");

	printf("Script:\n-------------\n%s\n\n", p2->pScript);
	printf("Expression:  %s\n", p2->pExpr);
	printf("ExtraParams: %s\n", p2->pExtraParams);
	printf("Signature:   %s\n", p2->pSignature);
	printf("Test Line:   %s\n", p2->pLine1);
	printf("Test Line:   %s\n", p2->pLine2);
	printf("Test Line:   %s\n", p2->pLine3);
	printf("crc32:       %08x\n", p2->crc32);
	if (nConst) {
		int i;
		for (i = 1; i <= nConst; ++i)
			printf("Const%d:      %s\n", i, Const[i]);
	}
	return 0;
}
#endif

#endif /* DYNAMIC_DISABLED */