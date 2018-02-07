#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>


/* 本程序使用的函数有：
   bool has_colors(void);		//判断终端是否支持色彩显示
   int start_color(void);		//该函数用来初始化颜色显示功能
   int init_pair(short pair_number, short foreground, short background);	//对颜色组合进行初始化
   int COLOR_PAIR(int pair_number);	//将颜色组合当做属性来访问

   本程序使用到的全局变量有：
	COLORS：	 当前终端可用颜色数目的最大值
	COLOR_PAIRS：当前终端支持的颜色组合的最大值
*/

int main()
{
	int i;

	initscr();

	/* 首先检查运行该程序的终端是否支持彩色显示 */
	if (!has_colors())
	{
		endwin();
		fprintf(stderr, "Error - no color support on this terminal\n");
		exit (1);
	}

	/* 采用色彩显示 */
	if (start_color() != OK)
	{
		endwin();
		fprintf(stderr, "Error - could not initialize colors\n");
		exit (2);
	}

	/* 打印出终端可用颜色数目的最大值，及支持的颜色组合的最大值 */
	clear();
	mvprintw(5, 5, "There are %d COLORS, and %d COLOR_PAIRS available", COLORS, COLOR_PAIRS);
	refresh();

	/* 创建7个颜色组合，并通过for循环依次显示 */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_GREEN);
	init_pair(3, COLOR_GREEN, COLOR_RED);
	init_pair(4, COLOR_YELLOW, COLOR_BLUE);
	init_pair(5, COLOR_BLACK, COLOR_WHITE);
	init_pair(6, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(7, COLOR_CYAN, COLOR_WHITE);

	for (i = 1;i <= 7;i++)
	{
		attroff(A_BOLD);
		attrset(COLOR_PAIR(i));
		mvprintw(5 + i, 5 , "color pair %d" , i);
		attrset(COLOR_PAIR(i) | A_BOLD);
		mvprintw(5 + i, 25, "Bold color pair %d", i);
		refresh();
		sleep(1);
	}
	endwin();
	exit (EXIT_SUCCESS);
}
