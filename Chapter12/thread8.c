#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 6

void *thread_function(void *arg);

/* 修复thread8.c中的一些BUG */

int main()
{
	int res;
	pthread_t a_thread[NUM_THREADS];
	void *thread_result;
	int lots_of_threads;

	/* 通过循环，创建多个线程 */
	for (lots_of_threads = 0; lots_of_threads < NUM_THREADS;lots_of_threads++)
	{
		res = pthread_create(&(a_thread[lots_of_threads]), NULL, thread_function, (void *)&lots_of_threads);
		if (res != 0)
		{
			perror("Thread creation failed");
			exit (EXIT_FAILURE);
		}
		sleep(1);
	}

	printf("Waiting for threads to finish……\n");
	
	/* 以和创建线程相反的顺序同步线程 */
	for (lots_of_threads = NUM_THREADS - 1;lots_of_threads >= 0;lots_of_threads--)
	{
		res = pthread_join(a_thread[lots_of_threads], &thread_result);
		if (res == 0)
		{
			printf("Picked up a thread\n");
		}
		else
		{
			perror("pthread_join failed");
		}
	}
	printf("All done\n");
	exit (EXIT_SUCCESS);
}


void *thread_function(void *arg)
{
	int my_number = *(int *)arg;
	int rand_num;

	printf("thread_function is running. Argument was %d\n",my_number);

	/* 每个线程等待随机时间后退出 */
	rand_num = 1 + (int)(9.0 * rand() / (RAND_MAX + 1.0));
	sleep(rand_num);
	printf("Bye from %d\n", my_number);
	pthread_exit (NULL);
}
