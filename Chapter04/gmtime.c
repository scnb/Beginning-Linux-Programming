#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
	struct tm *tm_ptr;		//tm结构中包含分开的时、分、秒、日、月、年、星期、年份中的日期、是否使用夏令时等
	time_t the_time;

	(void) time(&the_time);
	//tm_ptr = gmtime(&the_time);		//通过gmtime()函数将纯秒数转换为tm结构
	tm_ptr = localtime(&the_time);
	/* 如果运行该函数时，在格林尼治标准时间(GMT)之外的时区，或者采用了夏令时，则时间有可能不正确。*/
	/* 若想返回正确的时间，可以使用localtime()函数来查看当地时间，其返回的值已根据当地时区和是否使用夏令时调整 */
	printf("Raw time is %ld\n", the_time);
	printf("gmtime gives:\n");
	printf("date: %02d/%02d/%02d\n",tm_ptr->tm_year,tm_ptr->tm_mon+1,tm_ptr->tm_mday);
	printf("time: %02d:%02d:%02d\n",tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec);
	exit (0);
}
