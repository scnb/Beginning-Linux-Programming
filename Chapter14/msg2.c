#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/msg.h>

/* 该程序用于发送消息 */

#define MAX_TEXT 512

struct my_msg_st
{
	long int my_msg_type;
	char some_text[MAX_TEXT];
};

int main()
{
	int running = 1;
	struct my_msg_st some_data;
	int msgid;
	char buffer[BUFSIZ];

	/* 创建消息队列，使用关键字1234 */
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if (msgid == -1)
	{
		fprintf(stderr, "msgget faield with error: %d\n", errno);
		exit (EXIT_FAILURE);
	}

	while (running)
	{
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = 1;
		strcpy(some_data.some_text, buffer);

		/* 调用msgsnd函数向消息队列中发送消息 */
		if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit (EXIT_FAILURE);
		}

		/* 如果检测到字符串end，则退出程序 */
		if (strncmp(buffer, "end", 3) == 0)
		{
			running = 0;
		}
	}
	exit (EXIT_SUCCESS);
}
