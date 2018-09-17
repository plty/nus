# CS3210 Lab 2 Parallel Programming with OpenMP and Performance Instrumentation

[TOC]

## Intro

**Every code and raw-data can be reviewed on my [GitHub](https://github.com/plty/nus/tree/master/enrolled/parallel-CS3210/labs/lab-2) or in more readable format and charts in [Google Sheets](https://docs.google.com/spreadsheets/d/18EC3T8OhoYRfh25xPqSjzx2D1GmrMs-V5_KYySRSUOE/edit?usp=sharing) **

## Lab 

Because of the i7 machines in Parallel Computing Lab can't run `perf` every machine is shared between 2 user. This resource sharing can't ensure that both parties not using the machine simultaneously and may introduce some disturbance in the data. Because of this I took another shot of collecting data on Thursday, All of the data presented in this paper is not taken on the Lab day. if you would like to see the old-data you can open it on `lab-data/old-data` 

## Excercise 4

### Instruction per cycles

### Mflops

## Exercise 5

In `mm-omp.c` the multiplication is done not purely row-major order but something in between of column-major order and row-major order. because of that, I created `mm-omp-row.c` ,  `mm-omp-col.c`   and renaming `mm-omp.c` into `mm-omp-ori.c`

In this excercise to simplify the code and data observation to see the effect of "cache-misses" I assume for the respective code: 

`mm-omp-ori.c` is calculating $A \times B$

`mm-omp-row.c` is calculating $A \times B^T $

`mm-omp-col.c` is calculating $A^T \times B$

such that the data that we got doesn't include the transposing part of the matrix.

## Excercise 6

Instead of recording data for $2^k \leq 64$ threads, I gather data for all values of $n \leq 64$ threads. 

### Runtime

The runtime is quite predictable and expected as $n > 20$ there are no gain in performance anymore in which speedup is stagnated. this is expected as the number of threads in our Xeon machines is 20. 

<iframe width="806" height="498" seamless frameborder="0" scrolling="no" src="https://docs.google.com/spreadsheets/d/e/2PACX-1vRAsgeuSawUjsHMNbeipeEDGLvZXTRzu10yDMXaIKli3pgOO6dtslrFWDeyag6cNzPK86KUT0MR6Hp-/pubchart?oid=611277726&amp;format=interactive"></iframe>

