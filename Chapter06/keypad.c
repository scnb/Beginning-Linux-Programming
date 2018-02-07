#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

/* 该程序演示keypad模式的用法：按下Esc键后，程序将在一段延时里判断这个Esc是一个Escape转义序列还是一个单独的按键 */

#define LOCAL_ESCAPE_KEY 27

int main()
{
	int key;

	initscr();
	crmode();
	keypad(stdscr, TRUE);

	/* 关闭回显功能以防止在你按下方向键时发生移动，然后清屏并显示一些文本 */
	noecho();
	clear();
	mvprintw(5, 5, "Key pad demonstration. Press 'q' to quit");
	move(7, 5);

	refresh();
	key = getch();		//获取用户按下的按键

	while (key != ERR && key != 'q')
	{
		move(7, 5);
		clrtoeol();

		if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z'))		//判断用户按下的是不是常规的字母按键
		{
			printw("Key was %c", (char)key);
		}
		else
		{
			switch(key)					//检查有可能是哪种转义序列
			{
				case LOCAL_ESCAPE_KEY:	printw("%s", "Escape key");break;
				case KEY_END:			printw("%s", "END key"); break;
				case KEY_BEG:			printw("%s", "BEGINNING key");break;
				case KEY_RIGHT:			printw("%s", "RIGHT key");break;
				case KEY_LEFT:			printw("%s", "LEFT key");break;
				case KEY_UP:			printw("%s", "UP key");break;
				case KEY_DOWN:			printw("%s", "DOWN key");break;
				default:				printw("Unmatched - %d", key);break;
			}
		}
		refresh();
		key = getch();
	}
	endwin();
	exit (EXIT_SUCCESS);
}
