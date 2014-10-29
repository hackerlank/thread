#include "kthread.h"
#include <iostream>
#include "base/error.h"

void TestCreate();
void TestDestroy();
void TestTerminate();

int main() {
	std::cout << "hello, Test" << std::endl;	

	// Create
	TestCreate();

	// Destroy
	TestDestroy();

	// Terminate
	TestTerminate();

	std::cout << "Test Success!" << std:endl;
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
}

// 成功：0
// 错误
// 系统错误：1
void TestTerminate()
{}
