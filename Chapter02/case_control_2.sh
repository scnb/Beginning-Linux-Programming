#! /bin/sh

echo "Is it moringing? Please answer yes or no"

read timeofday

case "$timeofday" in 
	yes | y | Yes | YES ) echo "Good Morning";;
	no  | n | No  | NO  ) echo "Good Afternoon";;
	* ) echo "Sorry, answer not recognized";;
esac

exit 0
