/* 使用termios结构的密码程序 */
/* 涉及到一下函数：
   int tcgetattr(int fd, struct termios *termios_p);	用来初始化一个与终端对应的termios结构
   int tcsetattr(int fd, int actions, const struct termios *termios_p);	 重新配置终端接口
   参数actions控制修改方式：
   ① TCSANOW：立刻对值进行修改
   ② TCSADRAIN：等当前的输出完成之后再对值进行修改
   ③ TCSAFLUSH：等当前的输出完成之后再对值进行修改，也丢弃还未从read调用返回的当前可用的任何输入
*/

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

#define PASSWORD_LEN 8

int main()
{
	struct termios initialrsettings, newrsettings;
	char password[PASSWORD_LEN + 1];
	
	//调用tcgetattr来获取标准输入的当前设置
	tcgetattr(fileno(stdin), &initialrsettings);

	//对原始的设置值做一份副本，以便在程序结束时还原设置
	newrsettings = initialrsettings;
	newrsettings.c_lflag &= ~ECHO;		//使用 X &= ~FLAG 来清除变量中X中的FLAG标志定义的比特，其实等价于 X |= FLAG

	printf("Enter password: ");

	//用newrSETTINGS变量中的值设置属性并读取用户输入的密码
	if (tcsetattr(fileno(stdin), TCSAFLUSH, &newrsettings) != 0)	//用TCSAFLUSH丢弃用户在程序准备好读取数据之前输入的任何内容
	{
		fprintf(stderr, "Counld not set attributes\n");
	}
	else
	{
		fgets(password, PASSWORD_LEN, stdin);
		//用initialrsettings还原原来的终端设置值
		tcsetattr(fileno(stdin), TCSANOW, &initialrsettings);
		fprintf(stdout, "\nYou entered %s\n", password);
	}
	exit (0);
}
