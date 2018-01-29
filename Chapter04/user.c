#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	uid_t uid;
	gid_t gid;

	struct passwd *pw;
	uid = getuid();			//获取当前程序关联的用户ID，即启动程序的用户的ID
	gid = getgid();			//获取启动程序的用户所在组的ID

	printf("User is %s\n",getlogin());					//getlogin()函数返回当前用户关联的登录名
	printf("User IDs:uid=%d, gid=%d\n", uid, gid);

	pw = getpwuid(uid);			//用过uid参数来获得用户信息
	printf("UID passwd entry:\nname=%s, uid=%d, gid=%d, home=%s, shell=%s\n",pw->pw_name, pw->pw_uid, pw->pw_gid, pw->pw_dir, pw->pw_shell);

	pw = getpwnam("root");		//以用户登录名来查找用户信息
	printf("root passed entry:\n");
	printf("name=%s, uid=%d, gid=%d, home=%s, shell=%s\n",pw->pw_name, pw->pw_uid, pw->pw_gid, pw->pw_dir, pw->pw_shell);
	exit (0);
}
