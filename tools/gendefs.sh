#!/bin/bash
banned=("_fini" "_init")
text=$(nm -C -D "$@" | awk '{ if($2=="T") print $3 }' | sort)

for token in $text
do
    found=0
    case "${banned[@]}" in  *"$token"*) found=1 ;; esac
    if [ $found -ne 1 ]
    then
        echo "DEF_FRWD($token)"
    fi

done
