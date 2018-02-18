#include <stdio.h>
#include <stdlib.h>

/* 使用valgrind来运行编译出的程序，可以检测内存泄露和数组访问出错的问题 */

/* 命令为： valgrind --leak-check=yes -v ./checker */

int main()
{
	char *ptr = (char *)malloc(1024);
	char ch;

	/* 尝试未初始化就读取数据 */
	ch = ptr[1024];

	/* 在分配的内存块之后写数据 */
	ptr[1024] = 0;

	ptr = 0;
	exit (0);
}
