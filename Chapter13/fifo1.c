#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/* 使用mkfifo函数创建命名管道，用于在不相关的进程之间交换数据 */
/* 命名管道(named pipe)：一种特殊类型的文件，它在文件系统中以文件名的形式存在，但其行为与没有名字的管道类似 */
int main()
{
	int res = mkfifo("/tmp/my_fifo", 0777);
	if (res == 0)
	{
		printf("FIFO created\n");
	}
	exit (EXIT_SUCCESS);
}
