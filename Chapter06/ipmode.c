#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#define PW_LEN 256
#define NAME_LEN 256

int main()
{
	char name[NAME_LEN];
	char password[PW_LEN];
	const char *real_password = "xyzzy";
	int i = 0;

	initscr();

	move(5,10);
	printw("%s","Please login: ");

	move(7,10);
	printw("%s","User name: ");
	getstr(name);					//输入用户名字

	move(8,10);
	printw("%s","Password: ");
	refresh();

	cbreak();
	noecho();						//关闭回显，这样在输入密码时，就不会显示在屏幕上

	memset(password, '\0', sizeof(password));		//设置一块内存区域用于接受用户输入的密码
	while (i < PW_LEN)
	{
		password[i] = getch();		//读取键盘输入的字符
		if (password[i] == '\n')
			break;

		move(8,20+i);
		addch('*');					//在屏幕上输出*号，来表示加密 密码
		refresh();
		i++;
	}

	/* 打开回显，并判断用户输入的密码是否正确，并给出验证信息 */
	echo();				//重新打开键盘回显
	nocbreak();

	move(11,10);
	if (strncmp(real_password, password, strlen(real_password)) == 0)	//strncmp函数相对来说更安全，应为提供了最长字符限制
	{
		printw("%s", "Correct");
	}		
	else
	{
		printw("%s", "Wrong");
	}
	printw("%s"," password");
	refresh();
	sleep(2);

	endwin();
	exit(EXIT_SUCCESS);
}
