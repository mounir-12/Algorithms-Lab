#!/bin/bash

if [ "$1" = "" ]; then
	echo "Too few arguments..."
else
	for f in ./testsets/*.in
	do
		# extract file name
		fullname=${f##*/}
		name=${fullname%.*}
		echo "Executing $name..."
		time ./$1 < $f > ${f%.*}.myout
		output=$(diff ${f%.*}.out ${f%.*}.myout)
		if [ "$output" = "" ]; then
			echo "Success"; echo ""
		else
			echo "Fail"
			echo $output; echo ""
		fi
		rm ${f%.*}.myout
	done
fi
