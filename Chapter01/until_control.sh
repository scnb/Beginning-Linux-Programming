#! /bin/sh

until who | grep "$1" > /dev/null
do
	sleep 60
done

# now ring the bell and announce the expected user.

echo '\a'

echo "**** $1 has just logged in ****"

exit 0
