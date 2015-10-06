#ifndef __INC_GETTIMEOFDAY_H
#define __INC_GETTIMEOFDAY_H

struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

#ifndef _WINSOCKAPI_
typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;
#endif

int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif