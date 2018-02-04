#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

int main()
{
	const char witch_one[] = " First Witch";
	const char witch_two[] = " Second Witch  ";
	const char *scan_ptr;

	initscr();

	move(5,15);
	attron(A_BOLD);				//开启文本属性——加粗显示
	printw("%s","Macbeth");
	attroff(A_BOLD);			//关闭文本属性
	refresh();
	sleep(3);

	move(8,15);
	attron(A_STANDOUT);			//开启文本属性——突出/强调显示，字符终端最亮
	printw("%s","Thunder and Lightning");
	attroff(A_STANDOUT);		//关闭文本属性
	refresh();
	sleep(3);

	move(10,10);
	printw("%s","When shall we three meet again");
	move(11,23);
	printw("%s","In thunder, lighting, or in rain?");
	move(13,10);
	printw("%s", "When the hurlyburly's done,");
	move(14,23);
	printw("%s","When the battle's lost and won.");
	refresh();
	sleep(3);

	attron(A_DIM);										//开启文本属性——半亮显示
	scan_ptr = witch_one + strlen(witch_one) -1;
	while (scan_ptr != witch_one)
	{
		move(10,10);
		insch(*scan_ptr--);								//通过指针显示预先定义的字符串
	}
	scan_ptr = witch_two + strlen(witch_two) - 1;
	while (scan_ptr != witch_two)
	{
		move(13,10);
		insch(*scan_ptr--);
	}

	attroff(A_DIM);
	refresh();
	sleep(3);

	move(LINES -1 , COLS -1);							//将光标移动到屏幕的最后
	refresh();
	sleep(3);
	
	endwin();
	exit(EXIT_SUCCESS);
}
