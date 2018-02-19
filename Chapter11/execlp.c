#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* 参数列表示例 */
char *const ps_argv[] = {"ps", "ax", 0};

/* 作为示例的环境 */
char *const ps_envp[] = {"PATH=/bin:/usr/bin", "TERM=console", 0};

/* exec系列函数示例 */
/* 在本程序中，我们选择execlp函数作为示范 */
//execl("/bin/ps", "ps", "ax", 0);
//execlp("ps", "ps", "ax", 0);
//execle("/bin/ps", "ps", "ax", 0, ps_envp);

//execv("/bin/ps", ps_argv);
//execvp("ps", ps_argv);
//execve("/bin/ps", ps_argv, ps_envp);

int main()
{
	printf("Running ps with execlp\n");
	execlp("ps", "ps", "ax", 0);
	printf("Done.\n");
	exit (0);
}
