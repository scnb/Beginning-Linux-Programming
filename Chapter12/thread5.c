#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* 在本程序中，创建新线程并设置其属性为脱离状态，则可以允许第二个线程独立的完成工作，而无需原先的线程等待它 */

void *thread_function(void *arg);

char message[] = "Hello World";

/* 作为新线程是否结束的标志 */
int thread_finished = 0;

int main()
{	
	int res;
	pthread_t a_thread;

	pthread_attr_t thread_attr;

	/* 创建一个线程属性，并对其进行初始化 */
	res = pthread_attr_init(&thread_attr);
	if (res != 0)
	{
		perror("Attribute creation failed");
		exit (EXIT_FAILURE);
	}

	/* 将线程属性设置为脱离状态 */
	res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if (res != 0)
	{
		perror("Setting detached attribute failed");
		exit (EXIT_FAILURE);
	}	

	/* 在创建新线程时，要指明属性 */
	res = pthread_create(&a_thread, &thread_attr, thread_function, (void *)message);
	if (res != 0)
	{
		perror("Thread creation failed");
		exit (EXIT_FAILURE);
	}

	/* 属性用完后，对其进行回收 */
	(void)pthread_attr_destroy(&thread_attr);

	/* 等待新线程结束 */
	while (!thread_finished)
	{
		printf("Waiting for thread to say it's finished……\n");
		sleep(1);
	}

	printf("Other thread finished, bye!\n");
	exit (EXIT_SUCCESS);	
}	


void *thread_function(void *arg)
{	
	printf("thread_function is running. Argument was %s\n", (char *)arg);
	sleep(4);
	printf("Second thread setting finished flag, and exiting now\n");
	/* 对新线程结束标志赋值，该变量由两个线程共享 */
	thread_finished = 1;
	pthread_exit(NULL);
}
