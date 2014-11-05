#include "kthread.h"
#include "base/function.h"

#ifdef WIN32
static unsigned __stdcall MyThreadProc(void *pvArg)
#else
static void *MyThreadProc(void *pvArg)
#endif
{
	KThread *pThread = (KThread *)pvArg;
	pThread->ThreadFunction();

	return 0;
}

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
	Destroy();
}

// rework, need check m_pfnThread
void KThread::ThreadFunction()
{
	m_pfnThread(m_pvThreadArg);
}

// 成功：0
// 错误
// 系统错误：1
// 参数错误：2 传入的函数指针为空
int KThread::Create(KThreadFunction *pfnThread, void * pvArg)
{
	int nRet = 0;

	nRet = 2;
	KF_PROCESS_ERROR(pfnThread != NULL);

	m_pfnThread = pfnThread;
	m_pvThreadArg = pvArg;

	nRet = 1;
#ifdef WIN32
	unsigned uThreadId = 0;
	m_ThreadHandle = (HANDLE)_beginthreadex(
			NULL,			//SD
			0,				// initial stack size
			MyThreadProc,	// thread function
			(void*)this,	// thread arg
			0,				// create option
			(unsigned *)uThreadId	// identifier
			);
	KF_PROCESS_ERROR(m_ThreadHandle != NULL);
#else
	int nSysCode = -1;
	pthread_attr_t ThreadAttr;
	nSysCode = pthread_attr_init(&ThreadAttr);
	KF_PROCESS_ERROR(nSysCode == 0);

	// set stack 
	nSysCode = pthread_attr_setstacksize(&ThreadAttr, 256 * 1024);
	KF_PROCESS_ERROR(nSysCode == 0);

	nSysCode = pthread_create(&m_ThreadHandle, &ThreadAttr, MyThreadProc, this);
	KF_PROCESS_ERROR(nSysCode == 0);

	nSysCode = pthread_attr_destroy(&ThreadAttr);
	KF_PROCESS_ERROR(nSysCode == 0)
#endif

	nRet = 0;
Exit0:
	return nRet;
}

// 成功：0
// 错误
// 系统错误：1
// 对象未初始化：2
int KThread::Destroy()
{
	int nRet = 2;
	KF_PROCESS_ERROR(m_pfnThread != NULL);

	int nSysCode = 0;
	nRet = 1;
#ifdef WIN32
	KF_PROCESS_ERROR(m_ThreadHandle != NULL);
	nSysCode = WaitForSingleObject(m_pfnThread, INFINITE);	// 一直等
	KF_PROCESS_ERROR(nSysCode != WAIT_FAILED);

	nSysCode = CloseHandle(m_pfnThread);
	KF_PROCESS_ERROR(nSysCode != 0);
	m_pfnThread = NULL;
#else
	KF_PROCESS_ERROR(m_ThreadHandle != 0);
	nSysCode = pthread_join(m_ThreadHandle);
	KF_PROCESS_ERROR(nSysCode == 0);
	m_ThreadHandle = 0;
#endif

	nRet = 0;
Exit0:
	return nRet;
}
