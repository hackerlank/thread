#ifndef __KTHREAD_H__
#define __KTHREAD_H__

typedef void KThreadFunction(void* pvArg);

#include "base/include/error.h"
#include "base/include/commondefine.h"

#ifdef WIN32
#include "process.h"
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

int KThread_Sleep(unsigned uMilliseconds);

class KThread{
	public:
		int Create(KThreadFunction *pfnThread, void * pvArg);
		int Destroy();
		int Terminate(DWORD dwExitCode = 0);

		KThread();
		~KThread();

		void ThreadFunction();

	private:
		void _Clear();
		KThreadFunction *m_pfnThread;
		void *m_pvThreadArg;

#ifdef WIN32
		HANDLE m_ThreadHandle;
#else
		pthread_t m_ThreadHandle;
#endif
};

#endif
