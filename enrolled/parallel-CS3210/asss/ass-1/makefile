all: clean train-mpi train-mpi-debug train-omp train-omp-debug
train-mpi: 
	mpiCC -std=c++14 train-mpi.cpp -o train-mpi

train-mpi-debug:
	mpiCC -DDEBUG -std=c++14 train-mpi.cpp -o train-mpi-debug

train-omp: 
	g++ -fopenmp -std=c++14 train-omp.cpp -o train-omp

train-omp-debug:
	g++ -fopenmp -DDEBUG -std=c++14 train-omp.cpp -o train-omp-debug

clean: 
	rm -f train-mpi train-mpi-debug train-omp train-omp-debug
