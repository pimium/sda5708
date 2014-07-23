#!/bin/sh

for i in $(seq 9);
do
	echo "echo: $i"
	sleep 1
	echo $i > /dev/sevenseg
done

