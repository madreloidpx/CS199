#!/bin/bash
start=$1
end=$2
recursionLimit=$3
for i in $(seq $start $end)
do
    python3 main.py iterateThreshold -gd tests/network$i.dat -rl $recursionLimit -out output/nmiSum$i.txt -tcf tests/community$i.dat > output/log$i.txt
done