all:
	nvcc -arch=sm_50 -dc solution.cu hash.cu
	nvcc -arch=sm_50 -dlink solution.o hash.o -o link.o
	g++ solution.o hash.o link.o -L. -lcudart -o solution


clean: 
	rm *.o solution