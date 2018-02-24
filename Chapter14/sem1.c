#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/sem.h>
#include "semun.h"

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

int main(int argc, char *argv[])
{
	int i;
	int pause_time;
	char op_char = 'O';

	srand((unsigned int)getpid());

	/* 调用semget函数通过一个随意的键来获得一个信号量标识符 */
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

	/* 判断启动程序时，是否带有参数 */
	if (argc > 1)
	{
		/* 若程序带有参数，它就负责信号量的初始化工作 */
		/* set_semvalue函数见子函数中的注释 */
		if (!set_semvalue())
		{
			fprintf(stderr, "Failed to initialize semaphore\n");
			exit (EXIT_FAILURE);
		}
		/* 程序带有参数还可以决定要打印哪个字符 */
		op_char = 'X';
		/* 当前进程挂起2秒使得其他进程有时间被调用 */
		sleep(2);
	}

	/* 连续进行10次的进出临界区操作 */
	for (i = 0;i < 10;i++)
	{
		/* 进入临界区之前调用P操作 */
		if (!semaphore_p())
		{
			exit (EXIT_FAILURE);
		}
		printf("%c", op_char);
		fflush(stdout);
		/* 该进程挂起随机的时间 */
		pause_time = rand() % 3;
		sleep(pause_time);
		printf("%c", op_char);
		fflush(stdout);

		/* 退出临界区之前调用V操作 */
		if (!semaphore_v())
		{
			exit (EXIT_FAILURE);
		}

		pause_time = rand() % 2;
		sleep(pause_time);
	}

	/* 在循环完成之后，在终端打印结束信息 */
	printf("\n%d - finished\n", getpid());

	if (argc > 1)
	{
		/* 若该程序带有参数，则在退出并删除信号量之前，需要挂起一段时间，等待其他调用实例都执行完毕 */
		sleep(10);
		del_semvalue();
	}
	exit (EXIT_SUCCESS);
}


static int set_semvalue(void)
{
	/* 该函数是针对更通用的semctl函数的简化接口 */
	union semun sem_union;

	sem_union.val = 1;
	/* semctl函数根据command参数SETVAL把信号量初始化为一个已知的值，该值通过sem_union.val设置 */
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		return (0);
	}
	return (1);
}


static void del_semvalue(void)
{
	union semun sem_union;

	/* semctl函数通过command参数IPC_RMID来删除一个已经无需使用的信号量标识符 */
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
	{
		fprintf(stderr, "Failed to delete semaphore\n");
	}
}

static int semaphore_p(void)
{
	struct sembuf sem_b;

	/* 信号量编号一般取0 */
	sem_b.sem_num = 0;
	/* 在本次操作中需要改变的值，-1表示执行P操作 */
	sem_b.sem_op = -1;
	/* 通常设置为SEM_UNDO，操作系统在进程退出时会自动处理信号量 */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return (0);
	}
	return (1);
}


static int semaphore_v(void)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	/* 1表示执行V操作 */
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_v failed\n");
		return (0);
	}
	return (1);
}
