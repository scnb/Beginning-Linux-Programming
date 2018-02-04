#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <stdlib.h>

/* 在这个函数中，使用子窗口来卷动另一窗口里的部分内容，这也是子窗口最主要的用途 */

int main()
{
	WINDOW *sub_window_ptr;
	int x_loop;
	int y_loop;
	int counter;
	char a_letter = '1';

	initscr();

	for (y_loop = 0;y_loop < LINES - 1;y_loop++)
	{
		for (x_loop = 0;x_loop < COLS - 1;x_loop++)
		{
			mvwaddch(stdscr, y_loop, x_loop, a_letter);
			a_letter++;
			if (a_letter > '9')
			{
				a_letter = '1';
			}
		}
	}
	/* 创建一个新的卷动子窗口，且必须在刷新屏幕之前对父窗口调用touchwin函数 */
	sub_window_ptr = subwin(stdscr, 10, 20, 10, 10);
	scrollok(sub_window_ptr, 1);

	touchwin(stdscr);
	refresh();
	sleep(1);

	/* 删除子窗口中的内容，重新输出一些文字 */
	werase(sub_window_ptr);
	mvwprintw(sub_window_ptr,2, 0, "%s", "This window will now scroll");
	wrefresh(sub_window_ptr);
	sleep(1);

	for (counter = 1;counter < 10; counter++)
	{
		wprintw(sub_window_ptr, "%s", "This text is both wrapping and scrolling.");
		wrefresh(sub_window_ptr);
		sleep(1);
	}


	delwin(sub_window_ptr);

	touchwin(stdscr);
	refresh();
	sleep(1);
	endwin();
	exit (EXIT_SUCCESS);
}
