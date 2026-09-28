#!/bin/bash

check_services(){
   services=("ssh" "docker" "postgresql")

   for service in "${services[@]}"
   do 
      if systemctl is-active --quiet "$service"
      then
          echo "$service: $(systemctl is-active "$service")"
      else
          echo "$service: INACTIVE"
      fi
   done
}

check_services
