#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
	int i;
	time_t the_time;

	for (i = 1;i <= 10;i++)
	{
		the_time = time((time_t *)0);			//返回自1970.1.1 0点至今的秒数，是一个长整形，存储在time()函数中的参数中。
		printf("The time is %ld\n", the_time);
		sleep(2);
	}
	exit (0);
}
