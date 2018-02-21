#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);
/* 创建互斥量，用来保护工作区和变量time_to_exit */
pthread_mutex_t work_mutex;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int time_to_exit = 0;

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;
	/* 初始化互斥量 */
	res = pthread_mutex_init(&work_mutex, NULL);
	if (res != 0)
	{
		perror("Mutex initialization failed");
		exit (EXIT_FAILURE);
	}

	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if (res != 0)
	{
		perror("Thread creation failed");
		exit (EXIT_FAILURE);
	}
	
	/* 主线程里先对工作区进行加锁 */
	pthread_mutex_lock(&work_mutex);
	printf("Input some text. Enter 'end' to finish\n");
	while (!time_to_exit)
	{
		/* 输入文本并解锁，让新线程统计字符数目 */
		fgets(work_area, WORK_SIZE, stdin);
		pthread_mutex_unlock(&work_mutex);
		/* 判断新线程是否统计完（通过判断work_ared[0]是否为NULL）*/
		while (1)
		{
			pthread_mutex_lock(&work_mutex);
			/* 如果还未统计完，则保持解锁使新线程继续统计 */
			if (work_area[0] != '\0')
			{
				pthread_mutex_unlock(&work_mutex);
				sleep(1);
			}
			else
			{
				break;
			}	
		}
	}

	/* 在退出程序前，对工作区进行解锁 */
	pthread_mutex_unlock(&work_mutex);
	printf("\nWaiting for thread to finish……\n");
	res = pthread_join(a_thread, &thread_result);
	if (res != 0)
	{
		perror("Thread join failed");
		exit (EXIT_FAILURE);
	}

	printf("Thread joined\n");
	pthread_mutex_destroy(&work_mutex);
	exit (EXIT_SUCCESS);
}


void *thread_function(void *arg)
{
	sleep(1);
	/* 对工作区进行加锁，并开始统计 */
	pthread_mutex_lock(&work_mutex);
	while (strncmp("end", work_area, 3) != 0)
	{
		printf("You input %d characters\n", strlen(work_area) - 1);
		/* 统计完成之后，将work_area[0]设为NULL，向主线程表明统计完成 */
		work_area[0] = '\0';
		/* 解锁，使主线程能够继续读取输入 */
		pthread_mutex_unlock(&work_mutex);
		sleep(1);
		/* 周期性地尝试加锁，如果成功，则检查主线程是否又读入了字符；如果没有，则解锁互斥量继续等待 */
		pthread_mutex_lock(&work_mutex);
		while (work_area[0] == '\0')
		{
			pthread_mutex_unlock(&work_mutex);
			sleep(1);
			pthread_mutex_lock(&work_mutex);
		}
	}
	/* 做退出程序前的准备 */
	time_to_exit = 1;
	work_area[0] = '\0';
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(0);
}
