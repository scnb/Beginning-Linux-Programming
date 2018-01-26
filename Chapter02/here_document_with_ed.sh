#! /bin/sh

ed a_text_file <<!!
3
d
., \$s/is/was/
w
q
!!

exit 0
