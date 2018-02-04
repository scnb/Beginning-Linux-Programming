#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

int main()
{
	initscr();			//在开始使用curses库之前，先进行初始化
	move(5,15);			//将光标移动到（5,15）处
	printw("%s", "Hello, World");
	refresh();			//刷新物理屏幕
	sleep(2);
	endwin();			//结束使用后，恢复原先的设置	
	exit(EXIT_SUCCESS);
}
