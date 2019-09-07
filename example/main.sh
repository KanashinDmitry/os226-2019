#!/bin/bash

max_num_repl=0
freq_num=0

while read curr_num
do
	arrCounter[curr_num]=$(expr ${arrCounter[curr_num]} + 1)
	if [ $max_num_repl -lt ${arrCounter[curr_num]} ]
	then
		max_num_repl=${arrCounter[curr_num]}
		freq_num=$curr_num
	fi
done

echo $freq_num