all:	shdMem shdMemServer shdMemClient 

shdMem:	shdMemAndFork.c
	gcc -Wall shdMemAndFork.c -o shdMem

shdMemServer:	shdMemServer.c
	gcc -Wall shdMemServer.c -o shdMemServer

shdMemClient:	shdMemClient.c
	gcc -Wall shdMemClient.c -o shdMemClient

clean:
	rm -f shdMem shdMemServer shdMemClient 
