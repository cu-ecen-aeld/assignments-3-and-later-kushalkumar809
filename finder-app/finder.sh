#!/bin/sh

filesdir=$1
searchstr=$2

if [ $# -eq 2 ]; then
	
	if [ -d "$filesdir" ]; then
		x=$(find "$filesdir" -type f | wc -l)
		y=$(grep -r "$searchstr" "$filesdir" | wc -l)
		echo "The number of files are $x and the number of matching lines are $y"
		
	
	else
		echo "Directory not found"
		exit 1
	fi
else
	echo "Expected 2 arguments found $#"
	exit 1
fi
