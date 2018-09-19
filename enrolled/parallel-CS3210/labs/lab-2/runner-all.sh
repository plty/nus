#!/bin/bash
for i in {1..5}
do
    ./runner-ori.sh
    git add .
    git commit -m "new-data from soctf-7"
    git push

    ./runner-col.sh
    git add .
    git commit -m "new-data from soctf-7"
    git push

    ./runner-row.sh
    git add .
    git commit -m "new-data from soctf-7"
    git push
done
poweroff
