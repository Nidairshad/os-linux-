#!/bin/bash

REPORT="/var/www/mysite/reports/system-report.txt"

echo "================================" > "$REPORT"
echo " LINUX SYSTEM REPORT" >> "$REPORT"
echo "================================" >> "$REPORT"

echo >> "$REPORT"
echo "===== SYSTEM =====" >> "$REPORT"
uname -a >> "$REPORT"
hostnamectl >> "$REPORT"

echo >> "$REPORT"
echo "===== OS =====" >> "$REPORT"
cat /etc/os-release >> "$REPORT"

echo >> "$REPORT"
echo "===== MEMORY =====" >> "$REPORT"
free -h >> "$REPORT"

echo >> "$REPORT"
echo "===== DISK =====" >> "$REPORT"
df -h / >> "$REPORT"

echo >> "$REPORT"
echo "===== PROCESSES =====" >> "$REPORT"
ps aux --sort=-%cpu | head -10 >> "$REPORT"

echo >> "$REPORT"
echo "===== NETWORK =====" >> "$REPORT"
ip addr >> "$REPORT"

echo >> "$REPORT"
echo "===== LISTENING PORTS =====" >> "$REPORT"
ss -tulpn >> "$REPORT"

echo >> "$REPORT"
echo "===== SERVICES =====" >> "$REPORT"
systemctl is-active nginx >> "$REPORT"
systemctl is-active ssh >> "$REPORT"

echo >> "$REPORT"
echo "Report generated successfully."
