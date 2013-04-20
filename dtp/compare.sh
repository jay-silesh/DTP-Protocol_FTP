#!/bin/sh


if [ -n "$1"];  then
	source_file="hello.txt"
	dest_file="output.txt"
elif [ "$1" -eq 2 ]; then
	source_file="hello2.txt"
	dest_file="output1.txt"
else
	source_file="hello.txt"
	dest_file="output.txt"
fi



if [ -n "$1" -a -n "$2" ]; then
	if diff -q "$2" "$1"; then
    	echo "WORKING!!!!!!!!!!!!!!!!!!!"
	else
    	echo ""
	fi    
else
	if diff -q "$dest_file" "$source_file"; then
    	echo "WORKING!!!!!!!!!!!!!!!!!!!"
	else
    	echo ""
	fi
    
fi


