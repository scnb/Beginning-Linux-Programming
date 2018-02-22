#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread_function(void *arg);

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;

	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if (res != 0)
	{
		perror("Thread creation failed");
		exit (EXIT_FAILURE);
	}

	/* 主线程停止3秒后发送取消请求 */
	sleep(3);
	printf("Canceling thread……\n");
	res = pthread_cancel(a_thread);
	if (res != 0)
	{
		perror("Thread cancelation failed");
		exit (EXIT_FAILURE);
	}

	printf("Waiting for thread to finish……\n");
	res = pthread_join(a_thread, &thread_result);
	if (res != 0)
	{
		perror("Thread join failed");
		exit (EXIT_FAILURE);
	}
	exit (EXIT_SUCCESS);
}


void *thread_function(void *arg)
{
	int i, res;
	/* 设置自己的取消状态，--ENABLE 可以接受取消请求 */
	res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (res != 0)
	{
		perror("Thread pthread_setcancelstate failed");
		exit (EXIT_FAILURE);
	}

	/* 当取消状态设置好之后，线程进入第二个控制层次，设置取消类型 */
	/* 有两行类型：
	   ① PTHREAD_CANCEL_ASYNCHRONOUS,使得在接收到取消请求后立即采取行动
	   ② PTHREAD_CANCEL_DEFERRED，即延迟取消，使得在接收到取消请求后，一直等待直到线程执行了某些特定的函数
	*/
	res = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	if (res != 0)
	{
		perror("Thread pthread_setcanceltype failed");
		exit (EXIT_FAILURE);
	}

	printf("thread_function is running\n");
	for (i = 0; i < 10;i++)
	{
		printf("Thread is still running (%d)……\n", i);
		sleep(1);
	}

	pthread_exit(0);
}
