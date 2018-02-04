include <unistd.h>
#include <stdlib.h>
#include <curses.h>

int main()
{
	WINDOW *new_window_ptr;
	WINDOW *popup_window_ptr;

	int x_loop;
	int y_loop;
	char a_letter = 'a';

	initscr();

	/* 用字符填充基本窗口，然后刷新物理屏幕 */
	move(5,5);
	printw("%s","Testing multiple windows");
	refresh();

	for (y_loop = 0; y_loop < LINES -1 ;y_loop++)
	{
		for (x_loop = 0;x_loop < COLS - 1;x_loop++)
		{
			mvwaddch(stdscr, y_loop, x_loop, a_letter);
			a_letter++;
			if (a_letter > 'z')
				a_letter = 'a';
		}
	}
	/* 刷新屏幕 */
	refresh();
	sleep(2);

	/* 创建一个尺寸为10*20的新窗口，并添加一些文本，然后刷新物理屏幕 */
	new_window_ptr = newwin(10,20,5,5);
	mvwprintw(new_window_ptr, 2, 2, "%s", "Hello World");
	mvwprintw(new_window_ptr, 5, 2, "%s", "Notice how very long lines wrap inside the window");
	wrefresh(new_window_ptr);		//刷新窗口
	sleep(2);
	
	/* 重新将整个屏幕填充成数字 */
	a_letter = '0';
	for (y_loop = 0; y_loop < LINES -1; y_loop++)
	{
		for (x_loop = 0;x_loop < COLS - 1;x_loop++)
		{
			mvwaddch(stdscr, y_loop, x_loop, a_letter);
			a_letter++;
			if (a_letter > '9')
				a_letter = '0';
		}
	}
	refresh();
	sleep(2);
	
	/* 此时，只调用wrefresh来刷新窗口，则什么也不会发生，因为上面的改动是在stdscr中发生，而不是在新窗口发生的 */
	wrefresh(new_window_ptr);
	sleep(2);
	/* 使用touchwin函数强制curses重新绘制窗口，即使窗口的内容没有发生变化 */
	touchwin(new_window_ptr);
	wrefresh(new_window_ptr);
	sleep(2);

	/* 再增加另一个加框的重叠窗口 */
	popup_window_ptr= newwin(10,20,8,8);
	box(popup_window_ptr, '|', '-');				//使用竖线和短横线作为边框
	mvwprintw(popup_window_ptr,5,2,"%s","Pop Up Window");
	wrefresh(popup_window_ptr);
	sleep(2);

	/* 在屏幕上轮流显示这两个新窗口 */
	touchwin(new_window_ptr);
	wrefresh(new_window_ptr);
	sleep(2);
	wclear(new_window_ptr);
	wrefresh(new_window_ptr);
	sleep(2);
	delwin(new_window_ptr);
	touchwin(popup_window_ptr);
	wrefresh(popup_window_ptr);
	sleep(2);
	delwin(popup_window_ptr);
	touchwin(stdscr);
	refresh();
	sleep(2);
	endwin();
	exit (EXIT_SUCCESS);
}

