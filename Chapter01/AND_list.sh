#! /bin/sh

# touch命令判断文件是否存在，若不存在则创建之
touch file_one

rm -f file_two

if [ -f file_one ] && echo "hello" && [ -f file_two ] && echo " there"
then
	echo "in if"
else
	echo "in else"
fi

exit 0
