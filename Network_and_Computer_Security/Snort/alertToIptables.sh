#!/bin/bash

aux=0
while :
do
var=$(tail -n 1 log/alert.fast | awk  '{print $12}' | cut -d: -f1)
if [ "$var" != "$aux" ]; then
	iptables  -A INPUT -s "$var" -j DROP
	aux=$var
fi
done
exit 0
