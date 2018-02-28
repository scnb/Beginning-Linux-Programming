#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *host, **names, **addrs;
	struct hostent *hostinfo;

	/* 把host变量设置为程序的命令行参数，或者默认设置为用户主机的主机名 */
	if (argc == 1)
	{
		char myname[256];
		gethostname(myname, 255);
		host = myname;
	}
	else
	{
		host = argv[1];
	}

	/* 调用gethostbyname从主机数据库中通过主机名来获取主机的信息 */
	hostinfo = gethostbyname(host);
	/* 若不存在该主机，则显示错误消息 */
	if (!hostinfo)
	{
		fprintf(stderr, "cannot get info for host: %s\n", host);
		exit (1);
	}

	/* 显示主机名和可能的别名 */
	printf("results for host %s:\n", host);
	printf("Name: %s\n", hostinfo->h_name);
	printf("Aliases:");

	names = hostinfo->h_aliases;
	while (*names)
	{
		printf(" %s",*names);
		names++;
	}
	printf("\n");

	/* 判断被查询的主机是不是一个IP主机 */
	if (hostinfo->h_addrtype != AF_INET)
	{
		fprintf(stderr, "not an IP host!\n");
		exit (1);
	}

	/* 显示IP的地址列表 */
	addrs = hostinfo->h_addr_list;
	while (*addrs)
	{
		printf(" %s", inet_ntoa(*(struct in_addr *)*addrs));
		addrs++;
	}

	printf("\n");
	exit (0);
}
