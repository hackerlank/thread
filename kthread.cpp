#include "kthread.h"
#include "base/include/function.h"

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
	_Clear();
}

KThread::~KThread()
{
	if (m_pfnThread) {
		err_msg("Uninit KThread Object Before Destroy It");

		Terminate();
	}
}

// rework, need check m_pfnThread
void KThread::ThreadFunction()
{
	m_pfnThread(m_pvThreadArg);
}

void KThread::_Clear()
{
#ifdef WIN32
	m_ThreadHandle = NULL;
#else
	m_ThreadHandle = 0;
#endif

	m_pfnThread = NULL;
	m_pvThreadArg = NULL;
}

// 成功：0
// 错误
// 系统错误：1
// 参数错误：2 传入的函数指针为空
// 已初始化过：3
int KThread::Create(KThreadFunction *pfnThread, void * pvArg)
{
	int nRet = 0;
	unsigned uThreadId = 0;
	int nSysCode = -1;

	nRet = 2;
	KF_PROCESS_ERROR(pfnThread != NULL);

	nRet = 3;
	KF_PROCESS_ERROR(m_pfnThread == NULL && m_pvThreadArg == NULL);

	m_pfnThread = pfnThread;
	m_pvThreadArg = pvArg;

	nRet = 1;
#ifdef WIN32
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
	pthread_attr_t ThreadAttr;
	nSysCode = pthread_attr_init(&ThreadAttr);
	KF_PROCESS_ERROR(nSysCode == 0);

	// set stack 
	nSysCode = pthread_attr_setstacksize(&ThreadAttr, 256 * 1024);
	KF_PROCESS_ERROR(nSysCode == 0);

	nSysCode = pthread_create(&m_ThreadHandle, &ThreadAttr, MyThreadProc, this);
	KF_PROCESS_ERROR(nSysCode == 0);

	nSysCode = pthread_attr_destroy(&ThreadAttr);
	KF_PROCESS_ERROR(nSysCode == 0);
#endif

	nRet = 0;
ExitFailed:
	return nRet;
}

// 成功：0
// 错误
// 系统错误：1
// 对象未初始化：2
int KThread::Destroy()
{
	int nRet = 2;
	int nSysCode = 0;

	// check self state
	KF_PROCESS_ERROR(m_pfnThread != NULL);

	nRet = 1;
#ifdef WIN32
	KF_PROCESS_ERROR(m_ThreadHandle != NULL);
	nSysCode = WaitForSingleObject(m_ThreadHandle, INFINITE);	// 一直等
	KF_PROCESS_ERROR(nSysCode != WAIT_FAILED);

	nSysCode = CloseHandle(m_ThreadHandle);
	KF_PROCESS_ERROR(nSysCode != 0);
#else
	KF_PROCESS_ERROR(m_ThreadHandle != 0);
	nSysCode = pthread_join(m_ThreadHandle, NULL);
	KF_PROCESS_ERROR(nSysCode == 0);
#endif

	_Clear();

	nRet = 0;
ExitFailed:
	return nRet;
}

// 成功：0
// 错误
// 系统错误：1
// 对象未初始化：2
int KThread::Terminate(DWORD dwExitCode)
{
	int nRet = 2;
	int nSysCode = 0;
	KF_PROCESS_ERROR(m_ThreadHandle);

	nRet = 1;
#ifdef WIN32
	if (m_ThreadHandle)
	{
		nSysCode = TerminateThread(m_ThreadHandle, dwExitCode);
		KF_PROCESS_ERROR(nSysCode != 0);
	}
#else
	if (m_ThreadHandle)
	{
		pthread_cancel(m_ThreadHandle);  //android的ndk下不支持该函数，可以考虑 thread 自己调用 pthread_exit
	}
#endif

	_Clear();

	nRet = 0;
ExitFailed:
	return nRet;
}
