#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

/* 该程序演示了锁文件的使用 */
/* 使用在fcntl.h中定义的open系统调用，并且带有标志O_CREAT、O_EXCL，
   能够以一个原子操作同时完成两项工作：
   ① 确定文件不存在
   ② 创建该文件
*/

int main()
{
	int file_desc;
	int save_errno;

	file_desc = open("/tmp/LCK.test", O_RDWR | O_CREAT | O_EXCL, 0444);
	if (file_desc == -1)
	{
		save_errno = errno;
		printf("Open failed with error %d\n", save_errno);
	}
	else
	{
		printf("Open successed\n");
	}
	exit (EXIT_SUCCESS);
}
