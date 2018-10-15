# Introduction to Distributed-Memory Programming using MPI

## Exercise 1

The data distribution that is used in the given `mm-mpi.c` is distributed blockwise among processes.  Advantage apart from the trivialness of this method of distribution, distributing blockwise is better than doing it in cyclic manner. Because there is no good reason that we need a cyclic distribution, because the distribution is very fair where there isn't any "easy block" and "hard block" there is not even any branching in matrix multiplication such that there is no case that would harness the advantages of cyclic distribution. And the waiting part of cyclic will slow down the execution of the program. 

Although the blockwise distribution is better than cyclic, doing it with checquerboard distribution will be more efficient. In the current implementation to solve for a column, master process had to send the whole matrix B to the slave. chequerboard distribution is doing this more efficiently because to solve a block you don't need to send the whole matrix B to the other process but just the column. this reducing our problem of bottleneck in networking and message passing. and this method of chequerboard is objectively superior to the row-based blockwise distribution. 

## Exercise 2

To make it easier and more elegant to code I'll be doing it instead of multiplying  $A \times B$ my multiplication method will solve $A \times B^T$ I'll attach my implementation in the end of this paper.

## Exercise 3

My implementation work as long as the requested number of process is dividing the matrix fairly as in $\sqrt{x} \mid size$ and the original `mm-mpi.c` only can distribute among $x = 2^k$ slaves. s.t. we can only benchmark this fairly and objectively by dividing the problem into 4, 16 or 64 processes and with the size of the matrix as 2048. I don't know why in the lab I can't use more than 3 computers concurrently with MPI s.t. I can only benchmark it by sharing it among 4 and 16 processes, this data is collected on the Xeon Workstation.

