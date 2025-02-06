#!/bin/bash 
input=$1

while read -r line; do
    response=$(curl -m 1 "https://$line")
    exit_code=$!
    if  [[ exit_code -eq 0 ]]; then 
        echo "****************************************" >> output.txt
        echo "Get request for $line" >> output.txt
        echo "${response}" >> output.txt
        echo "****************************************" >> output.txt
    # echo "$line"
    fi
done <$1
