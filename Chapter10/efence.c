#include <stdio.h>
#include <stdlib.h>

int main()
{
	/* 分配一个内存块 */
	char *ptr = (char *)malloc(1024);
	ptr[0] = 0;

	/* 在上面分配的内存块后面写数据 */
	ptr[1024] = 0;
	exit (0);
}
