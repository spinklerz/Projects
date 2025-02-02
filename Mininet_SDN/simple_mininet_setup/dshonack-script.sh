#!/bin/bash
files=($(ls))
len=${#files[@]}
count=1
val=0
for((x=0; x<len; x++)); do
	lines="$(cat ${files[x]})"
	while read -r line; do 
		val=$((count % 2))
		if [[ $val -eq 0 ]]; then
			echo ${line}
		fi 
		((count++))
	done<${files[x]}
	count=1
done

