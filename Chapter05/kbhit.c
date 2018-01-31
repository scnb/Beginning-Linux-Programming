#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <term.h>
#include <curses.h>

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard();
void close_keyboard();
int kbhit();
int readch();

int main()
{
	/* 调用init_keyboard函数来配置终端，每隔1秒循环调用一次kbhit函数 */
	int ch = 0;
	init_keyboard();
	while (ch != 'q')
	{
		printf("looping\n");
		sleep(1);
		if (kbhit())
		{
			ch = readch();
			printf("you hit %c\n", ch);
		}
	}
	close_keyboard();
	exit (0);
}

/* init_keyboard函数在程序的开始对终端进行配置 */
void init_keyboard()
{
	tcgetattr(0, &initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
}


/* close_keyboard函数在程序的结束对终端配置还原 */
void close_keyboard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}

/* kbhit函数检测是否有按键按下 */
int kbhit()
{
	char ch;
	int nread;

	if (peek_character != -1)
	{
		return 1;
	}
	new_settings.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0, &ch, 1);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_settings);

	if (nread == 1)
	{
		peek_character = ch;
		return 1;
	}
	return 0;
}

/* readch函数读取按下的按键 */
int readch()
{
	char ch;

	if (peek_character != -1)
	{
		ch = peek_character;
		peek_character = -1;
		return ch;
	}
	read(0, &ch, 1);
	return ch;
}
