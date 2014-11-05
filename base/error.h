#ifndef __error_h_
#define __error_h_

#include <iostream>
#include <sys/types.h>
#include <string.h>
#ifdef WIN32
#include "stdarg.h"
#else
#include <unistd.h>
#endif

void err_sys(const char *fmt, ...);
void err_dump(const char* fmt, ...);
void err_quit(const char* fmt, ...);
void err_msg(const char* fmt, ...);
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap);

// 想想怎样才能输出这个值
#define K_ERROR_SYS( fmt, ... ) \
	do {\
		printf("SYS %s : at line %d in %s\n", #fmt, __LINE__, __FUNCTION__);\
	 	err_sys(fmt, ##__VA_ARGS__);\
	} while (false);\

#define K_ERROR_QUIT( fmt, ... )\
	do {\
		printf("QUIT %s : at line %d in %s\n", #fmt, __LINE__, __FUNCTION__);\
	 	err_quit(fmt, ## __VA_ARGS__);\
	} while(false);\

#endif
