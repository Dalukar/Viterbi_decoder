#!/bin/bash
c99 Main.c -o decode -lm
code=$1
if [ $# -eq 0 ]
then
	echo "No arguments, using test code 101010101010101010101"
	./decode 101010101010101010101
else
	./decode $code
fi


