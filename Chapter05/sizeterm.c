#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>

/* 该程序用到的系统函数为：
   int setupterm(char *term, int fd, int *errret);
	term：将当前终端类型值设置为参数term指向的值
	fd：文件描述符
	errret：函数的返回值保存在该参数指向的整型变量中
	该函数将当前终端类型设置为参数term执行的值

   int tigetnum(char *capname);
	capname：terminfo中数据项的名称
	该函数返回数值功能标志
*/
	
int main()
{
	int nrows, ncolumns;

	setupterm(NULL, fileno(stdout), (int *)0);
	nrows = tigetnum("lines");
	ncolumns = tigetnum("cols");
	printf("This terminal has %d cloumns and %d rows\n", ncolumns, nrows);
	exit (0);
}
