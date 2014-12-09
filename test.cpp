#include "kthread.h"
#include <iostream>
#include "base/include/error.h"
#include "exclusive.h"

void TestCreate();
void TestDestroy();
void TestTerminate();
void TestCalculate();

int main() {
	std::cout << "hello, Test" << std::endl;

	// Create
	TestCreate();

	// Destroy
	TestDestroy();

	// Terminate
	TestTerminate();

	// Test Thread
	TestCalculate();

	std::cout << "Test Success!" << std::endl;
	int nEnd;
	std::cin >> nEnd;
}

void TestThread(void* pVoid)
{
	int * x = (int *)pVoid;
	std::cout << "the value is" << *x << std::endl;
}

// 成功：0
// 错误
// 系统错误：1
// 参数错误：2 传入的函数指针为空
// 已初始化过：3
void TestCreate()
{
	KThread* pthread = new KThread();
	int nTestValue = 10086;
	int errcode = pthread->Create(NULL, NULL);
	// 创建一个空白函数的线程
	if (errcode != 2) {
		if (errcode == 1) {
			K_ERROR_SYS("error code :%d", errcode)
		} else {
			K_ERROR_QUIT("error code :%d", errcode)
		}
	}

	errcode = pthread->Create(TestThread, &nTestValue);
	if (errcode != 0){
		if (errcode == 1) {	// 和系统有关
			K_ERROR_SYS("error code: %d", errcode)
		} else {
			K_ERROR_QUIT("error code: %d", errcode)
		}
	}

	errcode = pthread->Create(TestThread, &nTestValue);
	if (errcode != 3)
	{
		if (errcode == 1)
		{
			K_ERROR_SYS("error code: %d", errcode)
		} else {
			K_ERROR_QUIT("error code: %d", errcode)
		}
	}

	pthread->Destroy();
	delete pthread;
}

// 成功：0
// 错误
// 系统错误：1
// 对象未初始化：2
void TestDestroy()
{
	KThread* pthread = new KThread();
	int nTestValue = 10086;

	int errcode = pthread->Destroy();
	if (errcode != 2){
		if (errcode == 1) {
			K_ERROR_SYS("error code :%d", errcode)
		} else {
			K_ERROR_QUIT("error code :%d", errcode)
		}
	}

	errcode = pthread->Create(TestThread, &nTestValue);
	if (errcode != 0){
		if (errcode == 1) {	// 和系统有关
			K_ERROR_SYS("error code: %d", errcode)
		} else {
			K_ERROR_QUIT("error code: %d", errcode)
		}
	}

	errcode = pthread->Destroy();
	if (errcode != 0) {
		if (errcode == 1) {
			K_ERROR_SYS("error code :%d", errcode)
		} else {
			K_ERROR_QUIT("error code :%d", errcode)
		}
	}
	delete pthread;
}

// 成功：0
// 错误
// 系统错误：1
// 对象未初始化：2
void TestTerminate()
{
	KThread* pthread = new KThread();
	int nTestValue = 10010;

	int errcode = pthread->Terminate();
	if (errcode != 2){
		if (errcode == 1) {
			K_ERROR_SYS("error code :%d", errcode)
		} else {
			K_ERROR_QUIT("error code :%d", errcode)
		}
	}

	errcode = pthread->Create(TestThread, &nTestValue);
	if (errcode != 0){
		if (errcode == 1) {	// 和系统有关
			K_ERROR_SYS("error code: %d", errcode)
		} else {
			K_ERROR_QUIT("error code: %d", errcode)
		}
	}

	errcode = pthread->Terminate();
	if (errcode != 0) {
		if (errcode == 1) {
			K_ERROR_SYS("error code :%d", errcode)
		} else {
			K_ERROR_QUIT("error code :%d", errcode)
		}
	}

	delete pthread;
}

void _CheckErrCode(int errcode, int nRightCode, int nSysErrCode)
{
	if (errcode != nRightCode){
		if (errcode == nSysErrCode) {	// 和系统有关
			K_ERROR_SYS("error code: %d", errcode)
		} else {
			K_ERROR_QUIT("error code: %d", errcode)
		}
	}
}

int nMainValue = 0;

void TestThread_Empty(void * pAddValue)
{
	std::cout << "id is" << *(int*)pAddValue << ", the main is" << nMainValue << std::endl;
	nMainValue += *((int *)pAddValue);
	std::cout << "id is" << *(int*)pAddValue << ", the main is" << nMainValue << std::endl;
}

void TestThread_Empty_Sleep1500(void * pAddValue)
{
	KThread_Sleep(1500);

	TestThread_Empty(pAddValue);
}

KExclusive Lock;

void TestThread_Lock_Sleep1000(void * pAddValue)
{
	Lock.EnterExclusive();
	
	KThread_Sleep(1000);
	TestThread_Empty(pAddValue);

	Lock.LeaveExclusive();
}

void TestCalculate()
{
	KThread* pThread = new KThread();
	int nId = 1;

	int nOldValue = nMainValue;
	int errcode = pThread->Create(TestThread_Empty, &nId);
	_CheckErrCode(errcode, 0, 1);

	KThread_Sleep(1000);	
	if (nMainValue != nId + nOldValue)
	{
		K_ERROR_QUIT("calculate error: the result should be %d, but it's %d", nOldValue + nId, nMainValue);
	}

	pThread->Destroy();
	delete pThread;

	// Test for time
	int nId1 = 1;
	int nId2 = 2;
	nMainValue = 0;
	nOldValue = nMainValue;
	KThread* pThread1 = new KThread();
	KThread* pThread2 = new KThread();
	errcode = pThread1->Create(TestThread_Empty, &nId1);
	_CheckErrCode(errcode, 0, 1);
	errcode = pThread2->Create(TestThread_Empty_Sleep1500, &nId2);
	_CheckErrCode(errcode, 0, 1);

	KThread_Sleep(1000);
	if (nMainValue != nId1 + nOldValue)
	{
		K_ERROR_QUIT("calculate error: the result should be %d, but it's %d", nOldValue + nId1, nMainValue);
	}

	KThread_Sleep(1000);
	if (nMainValue != nId1 + nId2 + nOldValue)
	{
		K_ERROR_QUIT("calculate error: the result should be %d, but it's %d", nOldValue + nId1 + nId2, nMainValue);
	}

	pThread1->Destroy();
	pThread2->Destroy();
	delete pThread1;
	delete pThread2;

	// Test For Exculsive
	nId1 = 1;
	nId2 = 2;
	nMainValue = 0;
	nOldValue = nMainValue;
	pThread1 = new KThread();
	pThread2 = new KThread();
	errcode = pThread1->Create(TestThread_Lock_Sleep1000, &nId1);
	_CheckErrCode(errcode, 0, 1);
	KThread_Sleep(100);
	errcode = pThread2->Create(TestThread_Lock_Sleep1000, &nId2);
	_CheckErrCode(errcode, 0, 1);

	KThread_Sleep(1500);
	if (nMainValue != nId1 + nOldValue)
	{
		K_ERROR_QUIT("calculate error: the result should be %d, but it's %d", nOldValue + nId1, nMainValue);
	}
		
	KThread_Sleep(100);
	if (nMainValue != nId1 + nOldValue)
	{
		K_ERROR_QUIT("calculate error: the result should be %d, but it's %d", nOldValue + nId1, nMainValue);
	}

	KThread_Sleep(1000);
	if (nMainValue != nId1 + nId2 + nOldValue)
	{
		K_ERROR_QUIT("calculate error: the result should be %d, but it's %d", nOldValue + nId1 + nId2, nMainValue);
	}
}
