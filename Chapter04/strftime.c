# define _XOPEN_SOURCE		//GNU库在默认情况下并未声明strptime函数，需要明确请求使用X/Open的标准功能，需要在time.h前添加该行
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	struct tm *tm_ptr, timestruct;
	time_t the_time;
	char buf[256];
	char *result;

	(void) time(&the_time);
	tm_ptr = localtime(&the_time);
	strftime(buf,256,"%A %d %B, %I:%S %p", tm_ptr);

	printf("strftime gives: %s\n",buf);

	/* 为了演示strptime的用法，构建一个包含日期和时间的字符串，然后调用strptime将原始时间和日期值提取并打印出来 */
	strcpy(buf,"Thu 26 July 2007, 17:53 will do fine");

	printf("calling strptime with :%s\n",buf);
	tm_ptr = &timestruct;		//将tm_prt重新赋值，相当于清空

	result = strptime(buf,"%a %d %b %Y, %R",tm_ptr);	//从buf中提取出原始时间和日期，存储在struct tm *结构中
	printf("strptime consumed up to: %s\n", result);	//strptime函数返回一个指针，指向转换过程处理的最后一个字符后面的字符

	printf("strptime gives:\n");
	printf("date: %02d/%02d/%02d\n", tm_ptr->tm_year%100, tm_ptr->tm_mon+1, tm_ptr->tm_mday);
	printf("time: %02d:%02d\n",tm_ptr->tm_hour,tm_ptr->tm_min);
	exit (0);
}
