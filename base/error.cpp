#include "error.h"

#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#define LOG_ERR 0
#else
#include <syslog.h>
#endif

// 守护进程标记
int daemon_proc;
#define MAXLINE 256

static void err_doit(int, int, const char*, va_list);

// 这个函数说明了 va_list va_start va_arg va_end 的用法
// err_doit 则展示了 vnspritf
int FindMax(int n, ...)
{
	int i;
	va_list ap;
	va_start(ap, n);
	int largest = va_arg(ap, int);	// 沿着 ap，向后移 int 对应的位数
	for (i = 1; i < n; i++)
	{
		int nTemp = va_arg(ap, int);
		largest = largest > nTemp ? largest : nTemp;
	}
	va_end(ap);

	return largest;
}

// 输出一个和系统相关的错误，并退出程序——设定 err_doit 第一参数为 1
void err_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);

	exit(1);
}

// 除了 err_sys 的输出，还加上一个 dump 文件
void err_dump(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	
	abort();
	exit(1);
}

// 输出一个和系统无关的错误，并退出程序
void err_quit(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);

	exit(1);
}

// 输出和 err_quit 一致，但不退出程序
void err_msg(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);

	return;
}

// errnoflag 设定为 1 时，会输出系统记录的错误值，也就是 errno 对应的字符串
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
	int errno_save, n;
	char buf[MAXLINE+1];

	errno_save = errno;
#ifdef HAVE_VSNPRINTF
	vsnprintf(buf, MAXLINE, fmt, ap);	// safe call
#else
	vsprintf(buf, fmt, ap);
#endif
	n = strlen(buf);
#ifdef WIN32
	sprintf_s(buf + n, MAXLINE - n, ", [last err]: %d", GetLastError());
#else
	if (errnoflag)
		snprintf(buf+n, MAXLINE - n, ", [err]: %s", strerror(errno_save));
#endif
	strcat(buf, "\n");

	if (daemon_proc) {
#ifndef WIN32
		syslog(level, buf);
#endif
	} else {
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}

	return;
}
