#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

/* 该程序演示了临界区的使用：
   临界区：一个程序在执行时，只需独占的占用某一资源很短的时间。

   需要在程序进入临界区之前使用open系统调用创建锁文件，然后在
   退出临界区时用unlink系统调用删除该锁文件
*/

const char *lock_file = "/tmp/LCK.test2";

int main()
{
	int file_desc;
	int tries = 10;

	while (tries--)
	{
		file_desc = open(lock_file, O_RDWR | O_CREAT | O_EXCL, 0444);
		if (file_desc == -1)
		{
			printf("%d - Lock already present\n", getpid());
			sleep(3);
		}
		else
		{
			/* 临界区从此处开始 */
			printf("%d - I have exclusive access\n", getpid());
			sleep(1);
			(void)close(file_desc);
			/* 退出临界区时，使用unlink函数删除该锁文件 */
			(void)unlink(lock_file);
			sleep(2);
		}
	}
	exit (EXIT_SUCCESS);
}
