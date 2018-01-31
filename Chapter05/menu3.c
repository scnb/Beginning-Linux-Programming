#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* 该程序将与用户交互的部分和其他的输入输出分开，只将输入和输出进行重定向，需要将与用户交互的部分与stdout、stderr分离开 */

//定义菜单字符串数组
char *menu[] = {"a - add new record","d - delete record","q - quit",NULL,};

int getchoice(char *greet, char *choices[], FILE *in, FILE *out);

int main()
{
	int choice = 0; 
	FILE *input;
	FILE *output;

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

	do
	{
		choice = getchoice("Please select an action", menu, input, output);	//显示菜单信息，并等待用户输入菜单选项首字母
		printf("You have chosen: %c\n", choice);	//默认输出到屏幕上
	} while(choice != 'q');							//直到用户输入q想要退出，否则一直循环
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
		} while (selected == '\n');

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
