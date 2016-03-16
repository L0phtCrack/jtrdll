#ifndef __INC_W32CONFIG_H
#define __INC_W32CONFIG_H

#define uint unsigned __int32
#define ushort unsigned __int16
#define INLINE __inline

#define get_power_of_two(v)		\
{								\
	v--;						\
	v |= v >> 1;				\
	v |= v >> 2;				\
	v |= v >> 4;				\
	v |= v >> 8;				\
	v |= v >> 16;				\
	v |= (v >> 16) >> 16;		\
	v++;						\
}


#endif
