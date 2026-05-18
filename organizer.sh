#!/bin/bash

mkdir -p Images PDFs scripts texts

for file in *
do
    # skip script itself, log, and directories
    if [[ "$file" == "$(basename "$0")" || "$file" == "log.txt" || -d "$file" ]]
    then
        continue
    fi

    if [[ -f "$file" && ( "$file" == *.jpg || "$file" == *.png ) ]]
    then
        if [[ ! -e Images/$file ]]
        then
            mv "$file" Images/
            echo "$file moved to Images" >> log.txt
        else
            echo "Image already exists: $file"
        fi

    elif [[ -f "$file" && "$file" == *.pdf ]]
    then
        if [[ ! -e PDFs/$file ]]
        then
            mv "$file" PDFs/
            echo "$file moved to PDFs" >> log.txt
        else
            echo "PDF already exists: $file"
        fi

    elif [[ -f "$file" && "$file" == *.sh ]]
    then
        if [[ ! -e scripts/$file ]]
        then
            mv "$file" scripts/
            echo "$file moved to scripts" >> log.txt
        fi

    elif [[ -f "$file" && "$file" == *.txt ]]
    then
        if [[ ! -e texts/$file ]]
        then
            mv "$file" texts/
            echo "$file moved to texts" >> log.txt
        fi
    fi
done
