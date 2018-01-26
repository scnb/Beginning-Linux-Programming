#! /bin/sh

echo "Enter password"
read trythis

while [ "$trythis" != "secret" ]
do
	echo "sorry, try again"
	read trythis
done

exit 0
