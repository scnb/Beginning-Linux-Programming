#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

/* 在本程序中，我们使用轮询技术，来验证两个线程的执行是同时进行的 */

/* 进程的所有线程，除局部变量外，所有其他变量都将在一个进程中的所有线程之间共享
*/

/* 编译该程序时，首先需要定义宏_REENTRANT，接下来必须链接正确的线程库。
   本系统默认使用的是NPTL线程库，则编译时无需加上-I 和 -L选项
   编译命令为：
   cc -D_REENTRANT thread2.c -o thread2 -lpthread
   完整的命令为：
   cc -D_REENTRANT -I/usr/include/nptl thread2.c -o thread2 -L/usr/lib/nptl -lpthread
*/

void *thread_function(void *arg);
int run_now = 1;

int main()
{
	int res;
	int print_count1 = 0;
	pthread_t a_thread;
	void *thread_result;

	/* 创建一个线程：
	   第一个参数：指向pthread_t类型数据的指针，指向的变量内容被写入一个标识符中，用来引用新线程
	   第二个参数：设置线程的属性，一般设置为NULL
	   第三个参数：新线程启动执行的函数指针
	   第四个参数：传递给上面函数的参数
	   注意：对于新线程来说，其将在启动函数处开始运行
	*/
	res = pthread_create(&a_thread, NULL, thread_function, (void *)message);
	if (res != 0)
	{
		perror("Thread creation failed");
		exit (EXIT_FAILURE);
	}

	/* 忙等待 */
	/* 变量run_now被两个线程共享，当其为1时，main线程输出1，并将其改为2；则新线程检测到该变量为2时，则输出2，并将其改为1 */
	while (print_count1++ < 20)
	{
		if (run_now == 1)
		{
			printf("1");
			run_now = 2;
		}
		else
		{
			sleep(1);
		}
	}

	printf("\nWaiting for thread to finish……\n");

	/* 等价于进程中用来收集子进程信息的wait函数
	   该函数将等到它所指定的线程终止后才返回
	   第一个参数：指定将要等待的线程，该线程通过pthread_create返回的标识符来指定
	   第二个参数：是一个指针，指向另一个指针，而后者指向线程的返回值
	*/	
	res = pthread_join(a_thread, &thread_result);
	if (res != 0)
	{
		perror("Thread join failed");
		exit (EXIT_FAILURE);
	}

	printf("Thread joined");
	exit (EXIT_SUCCESS);
}


/* 线程的启动函数 */
void *thread_function(void *arg)
{
	int print_count2 = 0;

	while (print_count2++ < 20)
	{
		if (run_now == 2)
		{
			printf("2");
			run_now = 1;
		}
		else
		{
			sleep(1);
		}
	}

	sleep(3);
}	
