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

echo "Top CPU processes: "
ps aux --sort=-%cpu | head -6

if [ -n "$1" ]; then
   echo
   echo "Searching for process : $1"

   pgrep -a "$1"
   if [ $? -eq 0 ]; then
      echo "process is running"
  else
     echo "process is not running"
 fi
fi

echo "====================="
echo "Network Troubleshooting check"
echo "====================="

echo

echo "[1]NETWORK INTERFACE"
echo "===================="
ip addr

echo "[2] Routing Table"
echo "-----------------"
ip route

echo
echo "[3] Neighbor Table"
echo "-----------------"
ip neigh

echo
echo "[4] Listening TCP/UDP Ports"
echo "---------------------------"
sudo ss -tulpn

echo
echo "[5] Active TCP Connections"
echo "--------------------------"
ss -tn

echo
echo "[6] Internect connectivity"
echo "--------------------------"
ping -c 4 8.8.8.8

echo
echo "[7] DNS Test"
echo "---------------------------"
nslookup google.com

echo
echo "-----------------------------"
echo "end of report"


