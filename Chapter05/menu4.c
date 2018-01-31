#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* 该程序将与用户交互的部分和其他的输入输出分开，只将输入和输出进行重定向，需要将与用户交互的部分与stdout、stderr分离开 */
/* 该程序基于menu3.c，做了如下修改：一旦输入字符，程序立即响应，而不用等到用户按下回车键进行确认						*/
/* 由于在非标准模式下，默认的回车符和换行符之间的映射不存在了，所以需要对回车符\r进行检查 								*/

//定义菜单字符串数组
char *menu[] = {"a - add new record","d - delete record","q - quit",NULL,};

int getchoice(char *greet, char *choices[], FILE *in, FILE *out);

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
	char **option;

	do		//不断循环直到用户输入正确的选项
	{
		fprintf(out, "Choice: %s\n", greet);	//输出到终端设备
		option = choices;
		while (*option)					//通过一个while循环，将字符串数组中的所有内容显示出来
		{
			fprintf(out, "%s\n", *option);
			option++;
		}

		do
		{
			selected = fgetc(in);			//从终端读取用户输入
		} while (selected == '\n' || selected == '\r');

		option = choices;
		while (*option)
		{
			if (selected == *option[0])	//判断用户输入是否符合菜单中某个选项
			{
				chosen = 1;
				break;
			}
			option++;
		}
		if (!chosen)
		{
			fprintf(out, "Incorrect choice, select again\n");
		}
	} while(!chosen);
	return selected;
}
