#!/bin/bash
a=$1
for file in $a*
do
#	echo $file
	com=$(./cr "$file" '1')
	output="$com"
	echo "${output}"

done
