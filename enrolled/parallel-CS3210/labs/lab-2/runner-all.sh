#!/bin/bash
for i in {1..3} 
do
    ./runner-ori.sh
    ./runner-col.sh
    ./runner-row.sh
done
