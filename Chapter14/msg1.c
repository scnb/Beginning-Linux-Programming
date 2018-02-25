#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/msg.h>

/* 该程序用于从消息队列中接收消息 */

struct my_msg_st
{
	long int my_msg_type;
	char some_text[BUFSIZ];
};

int main()
{
	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;

	/* 获取消息队列的标识符 */
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if (msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit (EXIT_FAILURE);
	}

	/* 从消息队列中读取消息，直到遇到end字符串为止 */
	while (running)
	{
		if (msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_receive, 0) == -1)
		{
			fprintf(stderr, "msgrcv failed with error :%d\n", errno);
			exit (EXIT_FAILURE);
		}

		printf("You wrote: %s", some_data.some_text);
		if (strncmp(some_data.some_text, "end", 3) == 0)
		{
			running = 0;
		}
	}

	/* 删除消息队列并退出 */
	if (msgctl(msgid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit (EXIT_FAILURE);
	}

	exit (EXIT_SUCCESS);
}
