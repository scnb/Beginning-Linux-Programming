#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include "shm_com.h"

/* 消费者程序将创建一个共享内存段，把写到它里面的数据都显示出来 */

int main()
{
	int running = 1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;	
	int shmid;

	srand((unsigned int)getpid());

	/* 调用shmget函数来创建一个共享内存，该函数返回共享内存标识符 */
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit (EXIT_FAILURE);
	}	

	/* 第一次创建共享内存后，要想使其能够被访问，必须将其连接到一个进程的地址空间中 */
	/* shmat调用成功后，将返回指向共享内存第一个字节的指针 */
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit (EXIT_FAILURE);
	}
	printf("Memory attached at %X\n", (int)shared_memory);

	shared_stuff = (struct shared_use_st *)shared_memory;
	shared_stuff->written_by_you = 0;
	while (running)
	{
		if (shared_stuff->written_by_you)
		{
			printf("You wrote: %s", shared_stuff->some_text);
			sleep(rand() % 4);
			shared_stuff->written_by_you = 0;
			/* 当读取到end字符串时，退出while循环 */
			if (strncmp(shared_stuff->some_text, "end", 3) == 0)
			{
				running = 0;
			}
		}
	}

	/* 将共享内存分离，并删除 */
	if (shmdt(shared_memory) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit (EXIT_FAILURE);
	}

	/* 通过command参数IPC_RMID将内存段删除 */
	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit (EXIT_FAILURE);
	}

	exit (EXIT_SUCCESS);
}
