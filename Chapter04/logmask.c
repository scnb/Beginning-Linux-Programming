#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*	void openlog(const char *ident, int logopt, int facility);
	调用opoenlog函数来改变日志信息的表示方式。
	ident:该字符串添加在日志信息的前面，可以通过该字符串来指明是哪个程序创建了这条信息
	logopt:对后续syslog()函数调用的行为进行配置
	facility:该参数记录一个将被用于后续syslog调用的默认设施值
*/

/* int setlogmask(int maskpri);
   调用setlogmask函数来设置一个日志掩码，并通过它来控制日志信息的优先级。
   maskpri有两种方式：
   ① LOG_MASK(priority)为日志信息创建一个掩码，它的作用是创建一个只包含一个优先级的掩码
   ② LOG_UPTO(priority)来创建一个指定优先级之上的所有优先级（包括指定优先级）构成的掩码
*/
int main()
{
	int logmask;

	openlog("logmask", LOG_PID|LOG_CONS,LOG_USER);
	syslog(LOG_INFO,"informative message,pid = %d", getpid());
	syslog(LOG_DEBUG, "debug message, should appear");
	logmask = setlogmask(LOG_UPTO(LOG_NOTICE));
	syslog(LOG_DEBUG, "debug message, should not appear");	//该语句的优先级为DEBUG，低于上一句设定的NOTICE，所以不会显示:w
	exit (0);
}
