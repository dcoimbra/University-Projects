#!/bin/bash
while true
do
#Please use 'man curl' to see what -vk is for, also -X command can be useful too.
curl -sk easyappointments:8888 > /dev/null
#sleep 5
done
exit 0
