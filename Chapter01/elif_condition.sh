#! /bin/sh

echo "Is it morning? Please answer yes or no"

read timeofday

# 变量的取值需要加上双引号，以防变量是空字符串或者其中存在空格
if [ "$timeofday" = "yes" ]
then
	echo "Good morning"
elif [ $timeofday = "no" ]
then
	echo "Good afternoon"
else
	echo "Sorry, $timeofday not recognized. Enter yes or no"
	exit 1
fi

exit 0
