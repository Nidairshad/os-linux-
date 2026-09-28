#!/bin/bash

   check_sys(){
   echo "system information:"
   uname -a
   hostname ctl
   cat /etc/os-release 
   }

   check_memory(){
   free -h
   }

   check_disk(){
    echo "Disk storage:"
    df -h /
   }

   check_processes(){
   ps aux
    }
   
   check_network(){
    ip addr
    }

    check_internet(){
    if ping -c 1 8.8.8.8 > /dev/null 2>&1
        then
            echo "Internet : CONNECTED"
        else
            echo "Internet : NOT CONNECTED"
        fi
    }

check_services() {
        read -p "Enter service name : " service
        status=$(systemctl is-active "$service")

        if [ "$status" = "active" ]
        then
            echo "$service: RUNNING"
        else
            echo "$service: NOT RUNNING"
        fi
}
    echo "================"
    echo "Trouble shooting kit" 
    echo "=============="

while true
do 
    echo "1.System information"
    echo "2.Disk storage check"
    echo "3.Memory check"
    echo "4.CPU/process check"
    echo "5.Find a process"
    echo "6.Network Interfaces"
    echo "7.Check Internet"
    echo "8.Check Services enabled/disabled"
    echo "0.Exit"
echo

read -p "Enter your choice: " choice
if [[ "$choice" =~ ^[0-8]$ ]]
then
    case "$choice" in

1)
   check_sys
  ;;
2)
   check_disk
   ;;
3)
   check_memory
   ;;
4)
    check_processes
   ;;
5)
  read -p "Enter process name: " process
  pgrep -af "$process"
  ;;
6)
   check_network
   ;;
7)
    check_internet
    ;;
8)
    check_services
    
   ;;
0)
  echo "Exiting .."
  break
  ;;
*)
   echo "Invalid choice"
   ;;
esac
else
echo "enter valid number"
fi
echo
read -p "press any key to continue .."
done
