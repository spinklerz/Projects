#!/bin/bash

domain=$1

theHarvester -d "${domain}" -b all -f outputTH.json
subfinder -d "${domain}" -oJ >> outputS.json

cat outputTH.json outputS.json > combined.json
jq -r '.host, .hosts[]?' combined.json | sort -u > subdomains.txt
jq -r '.asns[]?' combined.json | sort -u > asns.txt

jq -r '.ips[]?' combined.json | sort -u > ips.txt
jq -r '.source?' combined.json | sort -u > sources.txt
jq -r '.interesting_urls[]?' combined.json | sort -u > interesting_urls.txt

echo "Extraction complete. Files created:"
echo "- subdomains.txt"
echo "- asns.txt"
echo "- ips.txt"
echo "- sources.txt"
echo "- interesting_urls.txt"

