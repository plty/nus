#!/bin/bash
for i in {1..64}
do
    perf stat -o ex-4/stats-row --append -- ./mm-omp-row 2048 $i
    perf stat -o ex-4/stats-row-cache --append -e cache-references -e cache-misses -e cycles -e instructions -- ./mm-omp-row 2048 $i
done
