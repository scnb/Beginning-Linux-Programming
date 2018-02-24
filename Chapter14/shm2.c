#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include "shm_com.h"

/* 生产者程序连接一个共享内存段，并向其中写入数据 */

int main()
{
	int running = 1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	char buffer[BUFSIZ];
	int shmid;

	/* 调用shmget函数来创建一个共享内存段 */
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit (EXIT_FAILURE);
	}

	/* 第一次创建共享内存后，必须将其与进程的地址空间连接才能被访问 */
	/* 调用shmat函数成功后，返回指向共享内存的第一个字节的指针 */
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit (EXIT_FAILURE);
	}

	/* 显示上面函数调用后，共享内存与地址空间的哪个地方相连接 */
	printf("Memory attahed at %X\n", (int)shared_memory);
	shared_stuff = (struct shared_use_st *)shared_memory;
	while (running)
	{
		/* 该标志位为1时，表明消费者程序正在从共享内存读取数据 */
		while (shared_stuff->written_by_you == 1)
		{
			sleep(1);
			printf("waiting for client……\n");
		}
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);

		/* 向共享内存写入数据，并将标志位置1 */
		strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
		shared_stuff->written_by_you = 1;

		/* 当检测到输入中有end字符串时，结束读取字符串 */
		if (strncmp(buffer, "end", 3) == 0)
		{
			running = 0;
		}
	}

	/* 将共享内存与进程分离 */
	if (shmdt(shared_memory) == -1)
	{
		fprintf(stderr, "shamdt failed\n");
		exit (EXIT_FAILURE);
	}

	exit (EXIT_SUCCESS);
}
