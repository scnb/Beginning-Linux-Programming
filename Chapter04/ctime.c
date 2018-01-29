#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 使用ctime或者asctime函数来更友好的显示时间和日期 */
/* asctime()函数结构struct tm *结构，并返回一个包含时间和日期的字符串；
   ctime()函数结构一个纯秒数，并返回一个包含时间和日期的字符串
*/

int main()
{
	time_t timeval;
	struct tm *tm_ptr;
	(void) time(&timeval);
	tm_ptr = gmtime(&timeval);
	//printf("The data is: %s",ctime(&timeval));
	printf("The data is: %s\n",asctime(tm_ptr));
	exit (0);
}
