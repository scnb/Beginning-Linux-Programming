#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


/* 编译该程序时，首先需要定义宏_REENTRANT，接下来必须链接正确的线程库。
   本系统默认使用的是NPTL线程库，则编译时无需加上-I 和 -L选项
   编译命令为：
   cc -D_REENTRANT thread1.c -o thread1 -lpthread
   完整的命令为：
   cc -D_REENTRANT -I/usr/include/nptl thread1.c -o thread1 -L/usr/lib/nptl -lpthread
*/

void *thread_function(void *arg);

char message[] = "Hello World";

int main()
{
	int res;
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

	printf("Waiting for thread to finish……\n");

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

	printf("Thread joined, it returned %s\n", (char *)thread_result);
	printf("Message is now %s\n", message);
	exit (EXIT_SUCCESS);
}


/* 线程的启动函数 */
void *thread_function(void *arg)
{
	printf("thread_function is running.  Argument was %s\n", (char *)arg);
	sleep(3);
	strcpy(message, "Bye!");
	/* 该函数的作用是：终止调用它的线程并返回一个指向某个对象的指针 */
	pthread_exit("Thank you for the CPU time");
}	
