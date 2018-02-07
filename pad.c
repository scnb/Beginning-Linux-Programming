#include <unistd.h>
#include <stdlib.h>
#include <curses.h>


/* curses提供了一个特殊的数据结构pad来建立一种逻辑屏幕，该屏幕的尺寸大于正常物理屏幕。这样将逻辑屏幕的部分或全部内容输出到
   物理屏幕上，效果会更好。

   创建pad的方式为：
   WINDOW *newpad(int number_of_lines, int number_of_columns);

   删除pad的方式为：
   delwin，与删除正常的窗口一样

   pad使用不同的刷新操作，因为一个pad并不局限于某个特定的屏幕位置，必须指定希望放到屏幕上的pad范围及其放置在屏幕上的位置：
   int prefresh(WINDON *pad_ptr, int pad_row, int pad_column, int screen_row_min, int screen_col_min, int screen_row_max, int screen_col_max);

   上面的函数的作用为将pad从坐标（pad_row,pad_col）开始的区域写到屏幕上的指定区域，那个区域的范围是：(screen_row_min,screen_col_min) 到 (screen_row_max,screen_col_max)

*/

int main()
{
	/* 首先初始化pad结构，然后创建一个pad，之后用字符串填充这个pad */
	WINDOW *pad_ptr;
	int x, y;
	int pad_lines;
	int pad_cols;
	char disp_char;

	initscr();
	pad_lines = LINES + 50;
	pad_cols = COLS + 50;
	pad_ptr = newpad(pad_lines, pad_cols);
	disp_char = 'a';

	for (x = 0; x < pad_lines; x++)
	{
		for (y = 0; y < pad_cols; y++)
		{
			mvwaddch(pad_ptr, x, y, disp_char);
			if (disp_char == 'z')
			{
				disp_char = 'a';
			}
			else
			{
				disp_char++;
			}
		}
	}
	
	/* 将pad的不同区域绘制到屏幕的不同位置上 */
	prefresh(pad_ptr, 5, 7, 2, 2, 9, 9);
	sleep(1);
	prefresh(pad_ptr, LINES+5, COLS+7, 5,5, 21, 19);
	sleep(1);
	delwin(pad_ptr);
	endwin();
	exit (EXIT_SUCCESS);
}
