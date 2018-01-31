#include <stdio.h>
#include <stdlib.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <termios.h>

/* 该程序将与用户交互的部分和其他的输入输出分开，只将输入和输出进行重定向，需要将与用户交互的部分与stdout、stderr分离开 */
/* 该程序基于menu3.c，做了如下修改：一旦输入字符，程序立即响应，而不用等到用户按下回车键进行确认						*/
/* 由于在非标准模式下，默认的回车符和换行符之间的映射不存在了，所以需要对回车符\r进行检查 								*/

//定义菜单字符串数组
char *menu[] = {"a - add new record","d - delete record","q - quit",NULL,};

static FILE *output_stream = (FILE *)0;

int getchoice(char *greet, char *choices[], FILE *in, FILE *out);
int char_to_terminal(int char_to_write);

int main()
{
	int choice = 0; 
	FILE *input;
	FILE *output;

	struct termios initial_settings, new_settings;

	if (!isatty(fileno(stdout)))		//fileno将文件流转换成文件描述符，isatty判断该描述符是否连接到一个终端
	{
		fprintf(stderr, "You are not a terminal, OK.\n");	//错误信息输出到标准错误流
	}

	input = fopen("/dev/tty", "r");				//从终端设备读取输入
	output = fopen("/dev/tty", "w");			//向终端设备输出

	if (!input || !output)						//判断能否打开终端设备
	{
		fprintf(stderr, "Unable to open /dev/tty\n");
		exit (1);
	}

	//改变终端的特性
	tcgetattr(fileno(input), &initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_lflag &= ~ISIG;

	if (tcsetattr(fileno(input), TCSANOW, &new_settings) != 0)
	{
		fprintf(stderr, "could not set attributes\n");
	}
	
	do
	{
		choice = getchoice("Please select an action", menu, input, output);	//显示菜单信息，并等待用户输入菜单选项首字母
		printf("You have chosen: %c\n", choice);	//默认输出到屏幕上
		sleep(1);
	} while(choice != 'q');							//直到用户输入q想要退出，否则一直循环
	
	//在退出程序之前，需要将终端的设置值还原
	tcsetattr(fileno(input), TCSANOW, &initial_settings);
	exit (0);
}

/* 显示菜单，及读取用户输入 */
int getchoice(char *greet, char *choices[], FILE *in, FILE *out)
{
	int chosen = 0;
	int selected;
	//设置菜单在屏幕上的位置
	int screenrow = 10;
	int screencol = 55;
	char **option;
	char *cursor, *clear;

	output_stream = out;

	setupterm(NULL, fileno(out), (int *)0);
	cursor = tigetstr("cup");
	clear = tigetstr("clear");

	screenrow = 4;
	tputs(clear, 1, char_to_terminal);
	tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
	fprintf(out, "Choice: %s", greet);
	screenrow += 2;
	option = choices;
	while (*option)
	{
		tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
		fprintf(out, "%s", *option);
		screenrow++;
		option++;
	}
	fprintf(out, "\n");

	do		//不断循环直到用户输入正确的选项
	{
		fflush(out);
		selected = fgetc(in);
		option = choices;
		while (*option)					//通过一个while循环，将字符串数组中的所有内容显示出来
		{
			if (selected == *option[0])
			{
				chosen = 1;
				break;
			}
			option++;
		}
		if (!chosen)
		{
			tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
			fprintf(out, "Incorrect choice, select again\n");
		}
	} while(!chosen);
	tputs(clear, 1, char_to_terminal);
	return selected;
}

int char_to_terminal(int char_to_write)
{
	if (output_stream) 
	{
		putc(char_to_write, output_stream);
	}
	return 0;
}
