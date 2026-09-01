#!/bin/bash

echo "---------Linux process Monitor---------"

echo "\nSystem Information\n"

echo "User : $(whoami)"
echo "Hostname : $(hostname)"
echo "Shell : $SHELL"
echo "Uptime: $(uptime -p)"

echo

#current shell
echo "----------------------------------------------------------------------------"

echo "SHELL PID: $$"

ps -p $$ -f

echo

echo "PROCESS INFORMATION  "

echo 

ps aux --sort=-%cpu | head -6

echo

echo "TOP MEMORY PROCESSES "

ps aux --sort=-%mem | head -6

echo

echo "FIREFOX PROCESS "
echo

pgrep -a firefox

if [ $? -ne 0 ]; then
    echo "Firefox is not running."
fi

echo

echo "Firefox PIDS"

pidof Firefox

if [ $? -ne 0 ]; then
   echo "NO FIREFOX PROCESS FOUND."
fi

echo

echo "end of report"


