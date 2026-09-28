#!/bin/bash

usage=$(df / | awk 'NR==2 {print $5}' | tr -d '%')

echo "Disk usage: $usage%"

if [ "$usage" -ge 80 ]
then
   echo "WARNING: Disk usage is high"
else
   echo "Disk usage is OK"
fi
