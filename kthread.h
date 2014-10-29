#ifndef __KTHREAD_H__
#define __KTHREAD_H__

typedef void KThreadFunction(void* pvArg);

#ifdef WIN32
#include "process.h"
#else
#include <unistd.h>
#include <pthread.h>
#endif

class KThread{
	public:
		int Create(KThreadFunction *pfnThread, void * pvArg);
};

#endif
