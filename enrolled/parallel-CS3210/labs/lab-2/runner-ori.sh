#!/bin/bash
for i in {1..64}
do
    perf stat -o lab-data/stats-ori --append -- ./mm-omp-ori 2048 $i
    perf stat -o lab-data/stats-ori-cache --append -e cache-references -e cache-misses -e cycles -e instructions -- ./mm-omp-ori 2048 $i
done
