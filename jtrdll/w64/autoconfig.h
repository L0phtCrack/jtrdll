#ifndef __INC_AUTOCONFIG_H
#define __INC_AUTOCONFIG_H

/* autoconfig.h.  Generated from autoconfig.h.in by configure.  */
/* autoconfig.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* "Enable if CPU allows unaligned memory access" */
#define ARCH_ALLOWS_UNALIGNED 1

/* "Native CPU width, bits" */
#define ARCH_BITS 64

/* "Native CPU width, log" */
#define ARCH_BITS_LOG 6

/* "Native CPU width, string" */
#define ARCH_BITS_STR "64"

/* "Enable if sizeof(int) > 4" */
#define ARCH_INT_GT_32 0

/* "Enable if CPU is LITTLE ENDIAN arch" */
#define ARCH_LITTLE_ENDIAN 1

/* "Native CPU width" */
#define ARCH_SIZE 8

/* "Type with native size" */
#define ARCH_WORD long long

/* Define to 1 if you have the `AES_encrypt' function. */
#define HAVE_AES_ENCRYPT 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
/* #undef HAVE_ARPA_INET_H */

/* Define to 1 if you have the `atexit' function. */
#define HAVE_ATEXIT 1

/* Define to 1 if you have the `atoll' function. */
#define HAVE_ATOLL 1

/* Define to 1 if you have the `BIO_new' function. */
/* #undef HAVE_BIO_NEW */

/* Define to 1 if you have the `bzero' function. */
/* #undef HAVE_BZERO */

/* Define to 1 if you have the <CL/cl.h> header file. */
#define HAVE_CL_CL_H 1

/* Define to 1 if you have the CommonCrypto library. */
/* #undef HAVE_COMMONCRYPTO */

/* Define to 1 if you have the `crypt' library (-lcrypt). */
/* #undef HAVE_CRYPT */

/* Define to 1 if you have the <crypt.h> header file. */
/* #undef HAVE_CRYPT_H */

/* Define to 1 if you have the `endpwent' function. */
/* #undef HAVE_ENDPWENT */

/* Define to 1 if you have the `EVP_aes_256_xts' function. */
/* #undef HAVE_EVP_AES_256_XTS */

/* Define to 1 if you have the `EVP_sha512' function. */
/* #undef HAVE_EVP_SHA512 */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `floor' function. */
#define HAVE_FLOOR 1

/* Define to 1 if you have the `fopen64' function. */
/* #undef HAVE_FOPEN64 */

/* Define to 1 if you have the `fork' function. */
/* #undef HAVE_FORK */

/* Define to 1 if you have the `fseek64' function. */
/* #undef HAVE_FSEEK64 */

/* Define to 1 if you have the `fseeko' function. */
/* #undef HAVE_FSEEKO */

/* Define to 1 if you have the `fseeko64' function. */
/* #undef HAVE_FSEEKO64 */

/* Define to 1 if you have the `ftell64' function. */
/* #undef HAVE_FTELL64 */

/* Define to 1 if you have the `ftello' function. */
/* #undef  HAVE_FTELLO 1 */

/* Define to 1 if you have the `ftello64' function. */
/* #undef  HAVE_FTELLO64 */

/* Define to 1 if you have the `ftruncate' function. */
#define HAVE_FTRUNCATE 1

/* Define to 1 if you have the `gethostbyname' function. */
/* #undef HAVE_GETHOSTBYNAME */

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <gmp/gmp.h> header file. */
/* #undef HAVE_GMP_GMP_H */

/* Define to 1 if you have the <gmp.h> header file. */
/* #undef HAVE_GMP_H */

/* Define to 1 if you have Heimdal krb5. */
/* #undef HAVE_HEIMDAL */

/* Define to 1 if you have the <heimdal/heim_err.h> header file. */
/* #undef HAVE_HEIMDAL_HEIM_ERR_H */

/* Define to 1 if you have the <heim_err.h> header file. */
/* #undef HAVE_HEIM_ERR_H */

/* Define to 1 if you have the `inet_ntoa' function. */
/* #undef HAVE_INET_NTOA */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isascii' function. */
#define HAVE_ISASCII 1

/* Define to 1 if you have the Kerberos 5 libs. */
/* #undef HAVE_KRB5 */

/* Define to 1 if you have bz2 library and headers */
/* #undef HAVE_LIBBZ2 */

/* Define to 1 if you have the `crypto' library (-lcrypto). */
/* #undef HAVE_LIBCRYPTO */

/* Define to 1 if you have the `dl' library (-ldl). */
/* #undef HAVE_LIBDL */

/* Define to 1 if you have the `gmp' library (-lgmp). */
/* #undef HAVE_LIBGMP */

/* Define to 1 if you have the `kernel32' library (-lkernel32). */
/* #undef HAVE_LIBKERNEL32 */

/* Define to 1 if you have the `m' library (-lm). */
/* #undef HAVE_LIBM 1 */

/* Define to 1 if you have the `nsl' library (-lnsl). */
/* #undef HAVE_LIBNSL */

/* Define to 1 if you have the `rexgen' library (-lrexgen). */
/* #undef HAVE_LIBREXGEN */

/* Define to 1 if you have the `rt' library (-lrt). */
/* #undef HAVE_LIBRT */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the `ssl' library (-lssl). */
/* #undef HAVE_LIBSSL */

/* Define to 1 if you have the `ws2_32' library (-lws2_32). */
#define HAVE_LIBWS2_32 1

/* Define to 1 if you have the `wsock32' library (-lwsock32). */
#define HAVE_LIBWSOCK32 1

/* Define to 1 if you have the `wst' library (-lwst). */
/* #undef HAVE_LIBWST */

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `lseek' function. */
#define HAVE_LSEEK 1

/* Define to 1 if you have the `lseek64' function. */
/* #undef HAVE_LSEEK64 */

/* Define to 1 if you have the `memmem' function. */
/* #undef HAVE_MEMMEM */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mkdir' function. */
#define HAVE_MKDIR 1

/* Define to 1 if you have Heimdal with MKShim. */
/* #undef HAVE_MKSHIM */

/* Define to 1 if you have a working `mmap' system call. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the <netdb.h> header file. */
/* #undef HAVE_NETDB_H */

/* Define to 1 if you have the <netinet/if_ether.h> header file. */
/* #undef HAVE_NETINET_IF_ETHER_H */

/* Define to 1 if you have the <netinet/in.h> header file. */
/* #undef HAVE_NETINET_IN_H */

/* Define to 1 if you have the <netinet/in_systm.h> header file. */
/* #undef HAVE_NETINET_IN_SYSTM_H */

/* Define to 1 if you have the <netinet/ip.h> header file. */
/* #undef HAVE_NETINET_IP_H */

/* Define to 1 if you have the <net/ethernet.h> header file. */
/* #undef HAVE_NET_ETHERNET_H */

/* Define to 1 if you have the <net/if_arp.h> header file. */
/* #undef HAVE_NET_IF_ARP_H */

/* Define to 1 if you have the <net/if.h> header file. */
/* #undef HAVE_NET_IF_H */

/* Define to 1 if you have the <OpenCL/cl.h> header file. */
/* #undef HAVE_OPENCL_CL_H */

/* Define to 1 if you have the <OS.h> header file. */
/* #undef HAVE_OS_H */

/* Define to 1 if you have the <pcap.h> header file. */
/* #undef HAVE_PCAP_H */

/* Define to 1 if you have the <pcap/pcap.h> header file. */
/* #undef HAVE_PCAP_PCAP_H */

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD 1 */

/* Have PTHREAD_PRIO_INHERIT. */
/* #undef HAVE_PTHREAD_PRIO_INHERIT 1 */

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if you have the `rmdir' function. */
#define HAVE_RMDIR 1

/* Define to 1 if you have the `setenv' function. */
/* #undef HAVE_SETENV */

/* Define to 1 if you have the `SHA256' function. */
/* #undef HAVE_SHA256 */

/* Define to 1 if you have skey library and headers */
/* #undef HAVE_SKEY */

/* Define to 1 if you have the `sleep' function. */
/* #undef HAVE_SLEEP */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the `sprintf_s' function. */
#define HAVE_SPRINTF_S 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
/* #undef HAVE_STDINT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
/* #undef HAVE_STRCASECMP */

/* Define to 1 if you have the `strcmpi' function. */
#define HAVE_STRCMPI 1

/* Define to 1 if you have the `strcspn' function. */
#define HAVE_STRCSPN 1

/* Define to 1 if you have the `stricmp' function. */
#define HAVE_STRICMP 1

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlwr' function. */
#define HAVE_STRLWR 1

/* Define to 1 if you have the `strncasecmp' function. */
/* #undef HAVE_STRNCASECMP */

/* Define to 1 if you have the `strncmpi' function. */
/* #undef HAVE_STRNCMPI */

/* Define to 1 if you have the `strnicmp' function. */
#define HAVE_STRNICMP 1

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1

/* Define to 1 if you have the `strspn' function. */
#define HAVE_STRSPN 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL 1

/* Define to 1 if you have the `strupr' function. */
#define HAVE_STRUPR 1

/* Define to 1 if you have the <sys/ethernet.h> header file. */
/* #undef HAVE_SYS_ETHERNET_H */

/* Define to 1 if you have the <sys/file.h> header file. */
/* #undef HAVE_SYS_FILE_H */

/* Define to 1 if you have the <sys/param.h> header file. */
/* #undef HAVE_SYS_PARAM_H */

/* Define to 1 if you have the <sys/socket.h> header file. */
/* #undef HAVE_SYS_SOCKET_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/timeb.h> header file. */
/* #undef HAVE_SYS_TIMEB_H */

/* Define to 1 if you have the <sys/times.h> header file. */
/* #undef HAVE_SYS_TIMES_H */

/* Define to 1 if you have the <sys/time.h> header file. */
/* #undef HAVE_SYS_TIME_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <termios.h> header file. */
/* #undef HAVE_TERMIOS_H */

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define to 1 if you have the <unixlib/local.h> header file. */
/* #undef HAVE_UNIXLIB_LOCAL_H */

/* Define to 1 if you have the `vfork' function. */
/* #undef HAVE_VFORK */

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if you have the <wchar.h> header file. */
#define HAVE_WCHAR_H 1

/* Define to 1 if you have the `WHIRLPOOL' function. */
/* #undef HAVE_WHIRLPOOL */

/* Define to 1 if you have the <windows.h> header file. */
#define HAVE_WINDOWS_H 1

/* Define to 1 if `fork' works. */
/* #undef HAVE_WORKING_FORK */

/* Define to 1 if `vfork' works. */
/* #undef HAVE_WORKING_VFORK */

/* Define to 1 if you have the `_atoi64' function. */
#define HAVE__ATOI64 1

/* Define to 1 if the system has the type `_Bool'. */
/* #undef HAVE__BOOL */

/* Define to 1 if you have the `_fopen64' function. */
/* #undef HAVE__FOPEN64 */

/* Define to 1 if you have the `_fseeki64' function. */
#define HAVE__FSEEKI64 1

/* Define to 1 if you have the `_ftelli64' function. */
#define HAVE__FTELLI64 1

/* Define to 1 if you have the `_strcmpi' function. */
#define HAVE__STRCMPI 1

/* Define to 1 if you have the `_stricmp' function. */
#define HAVE__STRICMP 1

/* Define to 1 if you have the `_strncmpi' function. */
/* #undef HAVE__STRNCMPI */

/* Define to 1 if you have the `_strnicmp' function. */
#define HAVE__STRNICMP 1

/* Define to 1 if you want support for 4-byte UTF-8 in fast MS formats. */
#define NT_FULL_UNICODE 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "crioux@noctem.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "jtrdll"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "jtrdll"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "jtrdll"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "jtrdll"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE 8

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `int *', as computed by sizeof. */
#define SIZEOF_INT_P 8

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long double', as computed by sizeof. */
#define SIZEOF_LONG_DOUBLE 12

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `long long *', as computed by sizeof. */
#define SIZEOF_LONG_LONG_P 8

/* The size of `off_t', as computed by sizeof. */
#define SIZEOF_OFF_T 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 8

/* The size of `unsigned', as computed by sizeof. */
#define SIZEOF_UNSIGNED 4

/* The size of `unsigned char', as computed by sizeof. */
#define SIZEOF_UNSIGNED_CHAR 1

/* The size of `unsigned int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_INT 4

/* The size of `unsigned short', as computed by sizeof. */
#define SIZEOF_UNSIGNED_SHORT 2

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if you can safely include both <string.h> and <strings.h>. */
/*#undef STRING_WITH_STRINGS */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define this for the OpenCL Accelerator */
#define _OPENCL 1

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT32_T */

/* Define for Solaris 2.5.1 so the uint64_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT64_T */

/* Define for Solaris 2.5.1 so the uint8_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT8_T */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the type of a signed integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int32_t */

/* Define to the type of a signed integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int64_t */

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

/* Define to the type of an unsigned integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint16_t */

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint32_t */

/* Define to the type of an unsigned integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint64_t */

/* Define to the type of an unsigned integer type of width exactly 8 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint8_t */

/* Define as `fork' if `vfork' does not work. */
#define vfork fork

#endif