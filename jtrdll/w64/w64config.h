#ifndef __INC_W64CONFIG_H
#define __INC_W64CONFIG_H

//#define __SSE2__ 1
//#define __SSSE3__ 1
//#define __SSE4_1__ 1
//#define __AVX__ 1
//#define __XOP__ 1
//#define DES_BS_ASM 1

#define uint unsigned __int32
#define ushort unsigned __int16

#define FMT_SIZE_T "%Iu"
#define FMT_SSIZE_T "%Id"

#define INLINE __inline

INLINE unsigned long long get_power_of_two(unsigned long long v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= (v >> 16) >> 16;
	v++;

	return v;
}


#endif
