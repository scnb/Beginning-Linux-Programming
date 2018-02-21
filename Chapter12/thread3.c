#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

/* 程序一开始，将二进制信号量初始化为0，则新线程中的sem_wait函数将会阻塞
   在主线程中，等待用户从键盘输入，并通过sem_post函数将信号量加1，则新线
   程中的sem_wait函数返回并开始执行，在统计完字符个数之后，它再次调用sem_wait
   （信号量变为0）并再次被阻塞，直到主线程再次调用sem_post增加信号量的值
*/

void *thread_function(void *arg);
/* 创建二进制信号量 */
sem_t bin_sem;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;

	/* 将二进制信号量初始化为0，并且第二个参数为0表示是当前进程的局部信号量 */
	res = sem_init(&bin_sem, 0, 0);
	if (res != 0)
	{
		perror("Semaphore initialization failed");
		exit (EXIT_FAILURE);
	}

	/* 创建新线程 */
	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if (res != 0)
	{
		perror("Thread creation failed");
		exit (EXIT_FAILURE);
	}

	/* 向工作区中输入一些文本 */
	printf("Input some text. Enter 'end' to finish\n");
	/* 当没有读入end时，则继续读取文本 */
	/* strncmp：在work_area的前3个字节里搜寻是否存在end字符串 */
	while (strncmp("end", work_area, 3) != 0)
	{
		fgets(work_area, WORK_SIZE, stdin);
		/* 调用sem_post增加信号量的值 */
		sem_post(&bin_sem);
	}

	printf("\nWaiting for thread to finish……\n");
	res = pthread_join(a_thread, &thread_result);

	if (res != 0)
	{
		perror("Thread join failed");
		exit (EXIT_FAILURE);
	}


	printf("Thread joined\n");
	sem_destroy(&bin_sem);
	exit (EXIT_SUCCESS);
}


void *thread_function(void *arg)
{
	/* 在新线程中，等待信号量，统计来自输入的字符个数 */
	sem_wait(&bin_sem);
	while (strncmp("end", work_area, 3) != 0)
	{
		printf("You input %d characters\n", strlen(work_area) - 1);
		sem_wait(&bin_sem);
	}
	pthread_exit(NULL);
}
