#include "error.h"

int main() 
{
	// 测试宏，传 int 和 string 两种类型
	int errcode = 1;
	const char* errstring = "error_test";

	/* K_ERROR_QUIT("test errcode: %d", errcode); */
	K_ERROR_QUIT(errstring);
}
