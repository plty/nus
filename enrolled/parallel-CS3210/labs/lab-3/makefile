all: mm-mpi-cq mm-mpi-ori 

mm-mpi-cq: mm-mpi-cq.c
	mpicc -o mm-mpi-cq mm-mpi-cq.c -lm

mm-mpi-ori: mm-mpi-ori.c
	mpicc -o mm-mpi-ori mm-mpi-ori.c

clean:	
	rm -f mm-mpi-cq mm-mpi-ori

