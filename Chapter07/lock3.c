#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/* 该程序要和lock4.c程序配合使用 */

/* 该程序使用了fcntl函数，该函数对一个打开的文件描述符进行操作，并能根据command参数的设置完成不同的任务 
   
   int fcntl(int fildes, int command,……);
   对于第二个参数，有3个用于文件锁定的命令选项：
   ① F_GETLK
   ② F_SETLK
   ③ F_SETLKW
   当使用上面这些命令时，第三个参数必须是指向flock结构的指针。
   此时，该函数原型为：

   int fcntl(int fildes, int command, struct flock *flock_structure);
*/

const char *test_file = "/tmp/test_lock";

int main()
{
	int file_desc;
	int byte_count;
	char *byte_to_write = "A";
	struct flock region_1;
	struct flock region_2;
	int res;

	/* 打开一个文件描述符 */
	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if (!file_desc)
	{
		fprintf(stderr, "Unable to open %s for read/write\n", test_file);
		exit (EXIT_SUCCESS);
	}

	/* 向文件添加一些数据 */
	for (byte_count = 0;byte_count < 100; byte_count++)
	{
		(void)write(file_desc, byte_to_write, 1);
	}

	/* 把文件的10-30字节设为区域1，并在其上设置共享锁（读锁）*/
	region_1.l_type = F_RDLCK;
	region_1.l_whence = SEEK_SET;
	region_1.l_start = 10;
	region_1.l_len = 20;

	/* 把文件的40-50字节设为区域2，并在其上设置独占锁（写锁）*/
	region_2.l_type = F_WRLCK;
	region_2.l_whence = SEEK_SET;
	region_2.l_start = 40;
	region_2.l_len = 10;

	/* 锁定文件 */
	printf("Process %d locking file\n", getpid());
	res = fcntl(file_desc, F_SETLK, &region_1);
	if (res == -1)
	{
		fprintf(stderr, "Failed to lock region 1\n");
	}
	res = fcntl(file_desc, F_SETLK, &region_2);
	if (res == -1)
	{
		fprintf(stderr, "Failed to lock region 2\n");
	}

	sleep(60);
	printf("Process %d closing file\n", getpid());
	close(file_desc);
	exit (EXIT_SUCCESS);
}
