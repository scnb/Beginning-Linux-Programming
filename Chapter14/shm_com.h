/* 定义需要传输的文本长度为2K */
#define TEXT_SZ 2048

struct shared_use_st
{
	/* 当有数据写入这个结构时，用该整形变量通知消费者 */
	int written_by_you;
	char some_text[TEXT_SZ];
};


