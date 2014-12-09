#ifndef __EXCLUSIVE_H__
#define __EXCLUSIVE_H__

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#endif

class KExclusive
{
public:
	enum eTHREAD_MUTEX_TYPE {
		eTHREAD_MUTEX_NORMAL = 0,
#ifdef WIN32
		eTHREAD_MUTEX_RECURSIVE,	// 
#else
		eTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE,
#endif
	};

	KExclusive()
	{
#ifdef WIN32
		InitializeCriticalSection(&_mutex);
#else
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, (int)eType);
		pthread_mutex_init(&_mutex, &attr);
#endif
	}

	~KExclusive()
	{
#ifdef WIN32
		DeleteCriticalSection(&_mutex);
#else
		pthread_mutex_destroy(&_mutex);
#endif
	}

	// 进入独占模式
	unsigned int		EnterExclusive()
	{

#ifdef WIN32
		unsigned int uRet = 1;
		try
		{
			EnterCriticalSection(&_mutex);
		}
		catch (...)
		{
			uRet = 0;
			throw;
		}
		return uRet;
#else
		int nRet = pthread_mutex_lock(&_mutex);
		return nRet == 0;
#endif

	}

	// 离开独占模式
	unsigned int		LeaveExclusive()
	{

#ifdef WIN32
		unsigned int uRet = 1;
		try
		{
			LeaveCriticalSection(&_mutex);
		}
		catch (...)
		{
			uRet = 0;
			throw;
		}
		return uRet;
#else
		return pthread_mutex_unlock(&_mutex) == 0;
#endif
	}

protected:
private:
#ifdef WIN32
	CRITICAL_SECTION _mutex;
#else
	pthread_mutex_t _mutex;
#endif

};

#endif // __exclusive_h__
