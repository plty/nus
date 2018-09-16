#!/bin/bash
for i in {1..64}
do
    perf stat -o ex-4/stats-col --append -- ./mm-omp-col 2048 $i
    perf stat -o ex-4/stats-col-cache --append -e cache-references -e cache-misses -e cycles -e instructions -- ./mm-omp-col 2048 $i
done
