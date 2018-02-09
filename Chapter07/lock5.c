#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

const char *test_file = "/tmp/test_lock";

int main()
{
	int file_desc;
	struct flock region_to_lock;
	int res;

	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if (!file_desc)
	{
		fprintf(stderr, "Unable to open %s for read/write\n", test_file);
		exit (EXIT_FAILURE);
	}

	/* 指定文件的不同部分，并在其上执行不同的加锁操作 */

	/* 在文件的10-15字节上加共享锁 */
	region_to_lock.l_type = F_RDLCK;
	region_to_lock.l_whence = SEEK_SET;
	region_to_lock.l_start = 10;
	region_to_lock.l_len = 5;
	printf("Process %d, trying F_RDLCK, region %d to %d\n", getpid(),(int)region_to_lock.l_start, (int)(region_to_lock.l_start + region_to_lock.l_len));
	res = fcntl(file_desc, F_SETLK, &region_to_lock);
	if (res == -1)
	{
		printf("Process %d - Failed to lock region\n", getpid());
	}
	else
	{
		printf("Process %d - Obtained to lock region\n", getpid());
	}

	/* 将文件的10-15字节 解锁 */
	region_to_lock.l_type = F_UNLCK;
	region_to_lock.l_whence = SEEK_SET;
	region_to_lock.l_start = 10;
	region_to_lock.l_len = 5;
	printf("Process %d, trying F_UNLCK, region %d to %d\n",  getpid(), (int)region_to_lock.l_start, (int)(region_to_lock.l_start + region_to_lock.l_len));

	res = fcntl(file_desc, F_SETLK, &region_to_lock);
	if (res == -1)
	{
		printf("Process %d - Failed to unlock region\n", getpid());
	}
	else
	{
		printf("Process %d - Unlocked region\n", getpid());
	}


	/* 将文件的0-50字节 解锁 */
	region_to_lock.l_type = F_UNLCK;
	region_to_lock.l_whence = SEEK_SET;
	region_to_lock.l_start = 0;
	region_to_lock.l_len = 50;
	printf("Process %d, trying F_UNLKC, region %d to %d\n", getpid(), (int)region_to_lock.l_start, (int)(region_to_lock.l_start + region_to_lock.l_len));
	res = fcntl(file_desc, F_SETLK, &region_to_lock);
	if (res == -1)
	{
		printf("Process %d - Failed to unlock region\n", getpid());
	}
	else
	{
		printf("Process %d - Unlocked region\n", getpid());
	}


	/* 在文件的16-21字节上加独占锁 */
	region_to_lock.l_type = F_WRLCK;
	region_to_lock.l_whence = SEEK_SET;
	region_to_lock.l_start = 16;
	region_to_lock.l_len = 5;
	printf("Process %d, trying F_WRLCK, region %d to %d\n", getpid(), (int)region_to_lock.l_start, (int)(region_to_lock.l_start + region_to_lock.l_len));
	res = fcntl(file_desc, F_SETLK, &region_to_lock);
	if (res == -1)
	{
		printf("Process %d - Failed to lock region\n", getpid());
	}
	else
	{
		printf("Process %d - Obtained to lock region\n", getpid());
	}

	/* 在文件的40-50字节上加共享锁 */
	region_to_lock.l_type = F_RDLCK;
	region_to_lock.l_whence = SEEK_SET;
	region_to_lock.l_start = 40;
	region_to_lock.l_len = 10;
	printf("Process %d, trying F_RDLCK, region %d to %d\n", getpid(), (int)region_to_lock.l_start, (int)(region_to_lock.l_start + region_to_lock.l_len));
	res = fcntl(file_desc, F_SETLK, &region_to_lock);
	if (res == -1)
	{
		printf("Process %d - Failed to lock region\n", getpid());
	}
	else
	{
		printf("Process %d - Obtained to lock region\n", getpid());
	}

	/* 在文件的16-21字节上加独占锁 */
	region_to_lock.l_type = F_WRLCK;
	region_to_lock.l_whence = SEEK_SET;
	region_to_lock.l_start = 16;
	region_to_lock.l_len = 5;
	printf("Process %d, trying F_WRLCK, region %d to %d\n", getpid(), (int)region_to_lock.l_start, (int)(region_to_lock.l_start + region_to_lock.l_len));
	/* 使用F_SETLKW，将一直等待直到可以加锁为止 */
	res = fcntl(file_desc, F_SETLKW, &region_to_lock);
	if (res == -1)
	{
		printf("Process %d - Failed to lock region\n", getpid());
	}
	else
	{
		printf("Process %d - Obtained to lock region\n", getpid());
	}
	
	printf("Process %d ending\n", getpid());
	close(file_desc);
	exit (EXIT_SUCCESS);
}
