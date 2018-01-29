#include <sys/utsname.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char computer[256];
	struct utsname uts;
	/* gethostname()函数把机器的网络名写入computer字符串，该字符串至少有255个字符；
	   uname()函数把主机信息写入uts指向的结构体中
	 */
	if (gethostname(computer, 255) != 0 || uname(&uts) < 0)
	{
		fprintf(stderr, "Could not get host information\n");
		exit (1);
	}

	printf("Computer host name is %s\n", computer);						//输出主机名
	printf("System is %s on %s hardware\n", uts.sysname, uts.machine);	//输出操作系统名、硬件类型
	printf("Nodename is %s\n", uts.nodename);							//输出主机名
	printf("Version is %s, %s\n", uts.release, uts.version);			//输出系统发行级别、系统版本号
	exit (0);
}
