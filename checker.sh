#!/bin/bash

dirs=("assign" "output")
files=("cp_lib.cpp" "checker.sh")

pass=true

for dir in "${dirs[@]}"; do
    if [ ! -d "$dir" ]; then
        pass=false
        echo "$dir NOT FOUND"
    fi
done

for file in "${files[@]}"; do
    if [ ! -f "$file" ]; then
        pass=false
        echo "$file NOT FOUND"
    fi
done


outs=`ls ./output/`

regex=".txt$"

for out in $outs; do
    if [[ ! $out =~ $regex ]]; then
        pass=false
        echo "$out NOT REQUIRED"
    fi
done

var=`ls`

for name in $var; do
    temp=true
    for dir in "${dirs[@]}"; do
        if [ "$name" == "$dir" ]; then
            temp=false
        fi
    done
    for file in "${files[@]}"; do
        if [ "$name" == "$file" ]; then
            temp=false
        fi
    done
    if $temp; then
        pass=false
        echo "$name NOT REQUIRED"
    fi

done

if $pass; then
    echo "Accepted"
else
    echo "Rejected"
fi