#!/bin/bash
sleep 120
while true
do
#Please use 'man curl' to see what -vk is for, also -X command can be useful too.
curl -vk 172.18.0.4:8888
#sleep 5
done
exit 0
