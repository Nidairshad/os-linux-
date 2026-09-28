#!/bin/bash

for service in ssh docker postgresql
do
   echo "Checking $service..."
   systemctl is-active "$service"
   echo
done
