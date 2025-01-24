#!/bin/sh

writefile="$1"
writestr="$2"

if [ $# -eq 2 ] ; then
	mkdir -p "$(dirname $writefile)"
	touch "$writefile"
	echo "$writestr" > "$writefile"
	
	if [ -f "$writefile" ]; then
		echo "operation sucess"
		exit 0
	else
		echo " $writefile : Directory cannot be created"
		exit 1
	fi
else
	echo "failed : expected 2 arguments but instead got $#"
	exit 1
fi
