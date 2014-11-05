#include "kthread.h"
#include "base/function.h"

KThread::KThread()
{
#ifdef WIN32
	m_ThreadHandle = NULL;
#else
	m_ThreadHandle = 0;
#endif

	m_pfnThread = NULL;
	m_pvThreadArg = NULL;
}

KThread::~KThread()
{
	Destroy()
}

// 成功：0
// 错误
// 系统错误：1
// 参数错误：2 传入的函数指针为空
int KThread::Create(KThreadFunction *pfnThread, void * pvArg)
{
	int nRet = 0;

	nRet = 2;
	KF_PROCESS_ERROR(pfnThread == NULL);

	m_pfnThread = pfnThread;
	m_pvThreadArg = pvArg;

#ifdef WIN32
	unsigned uThreadId;
	m_ThradHandle = (HANDLE)_beginthreadex(
			NULL,	//SD
			0,	// initial stack size
			);

	nRet = 0;
Exit0:
	return nRet;
}
