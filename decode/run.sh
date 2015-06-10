#!/bin/bash
c99 Main.c -o decode -lm
code=$1
if [ $# -eq 0 ]
then
	echo "No arguments, using test code 101111001110001111111101"
	./decode 101111001110001111111101
else
	./decode $code
fi


