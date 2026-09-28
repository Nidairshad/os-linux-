#!/bin/bash

echo "---------Linux process Monitor---------"

echo "\nSystem Information\n"

echo "Current User : $(whoami)"
echo "Hostname : $(hostname)"
echo "Kernel : $(uname -r)"
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
echo "[8] DNS Configuration"
resolvectl status


echo "-----------------------------"
echo "end of report"


