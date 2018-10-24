all: train-omp train-mpi

train-omp: 
	g++ train-omp.cpp -std=c++14 -o train-omp
	./train-omp < test.in

train-mpi: 
	mpiCC train-mpi.cpp -std=c++14 -o train-mpi
	./train-mpi < test.in

clean: 
	rm -f train-omp train-mpi
