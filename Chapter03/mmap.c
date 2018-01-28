/* 利用mmap和数组方式的存取操作来修改一个结构化数据文件 */

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct			//定义一个RECORD结构 
{
	int integer;
	char string[24];
}RECORD;

#define NRECORDS (100)	//创建100个记录

int main()
{
	RECORD record, *mapped;
	int i, f;
	FILE *fp;

	fp = fopen("records.dat","w+");		//将100个记录写到文件records.dat中
	for (i=0;i<NRECORDS;i++)
	{
		record.integer = i;
		sprintf(record.string, "RECORD-%d",i);
		fwrite(&record,sizeof(record),1,fp);	//写入文件中
	}
	fclose(fp);

	/* change record 43's integer from 43 to 143 */
	fp = fopen("records.dat","r+");
	fseek(fp,43*sizeof(record),SEEK_SET);		//在文件中寻找第43条记录
	fread(&record,sizeof(record),1,fp);			//读取该条记录并存在record中

	record.integer = 143;
	sprintf(record.string,"RECORD-%d",record.integer);

	fseek(fp,43*sizeof(record),SEEK_SET);		//将修改后的record重新写入record第43条记录处
	fwrite(&record,sizeof(record),1,fp);
	fclose(fp);

	/* use another way to changer record's integer */
	/* map these records to memory, then access record 43, and change it's integer to 143, then update */

	f = open("records.dat",O_RDWR);				//获取文件描述符，在下面mmap函数中使用
	//以下参数依次为：内存起始地址、数据长度、模式、文件描述符、文件数据起始偏移地址
	mapped = (RECORD *)mmap(0, NRECORDS*sizeof(record),PROT_READ|PROT_WRITE,MAP_SHARED,f,0);
	mapped[43].integer = 243;
	sprintf(mapped[43].string,"RECORD-%d",mapped[43].integer);
	msync((void *)mapped, NRECORDS*sizeof(record), MS_ASYNC);	//该函数将整段内存中的数据写回到被映射的文件，采用同步写的方式
	munmap((void *)mapped, NRECORDS*sizeof(record));			//调用该函数释放上面使用的内存段
	close(f);
	exit (0);
}
