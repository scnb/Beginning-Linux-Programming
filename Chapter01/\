#! /bin/sh

trap 'rm -f /tmp/my_tmp_file_$$' INT
echo "creating file /tmp/my_tmp_file_$$"
date > /tmp/my_tmp_file_$$

echo "press interrupt (CTRL-C) to interrupt the script……"
while [ -f /tmp/my_tmp_file_$$ ]
do
	echo "File exists"
	sleep 1
done

echo "The file no longer exists"

trap INT

echo "creating file /tmp/my_tmp_file_$$"

date > /tmp/my_tmp_file_$$

echo "press interrupt (CTRL-C) to interrupt the script……"

while [ -f /tmp/my_tmp_file_$$ ]
do
	echo "File exists"
	sleep 1
done

echo "We never get here"

exit 0
