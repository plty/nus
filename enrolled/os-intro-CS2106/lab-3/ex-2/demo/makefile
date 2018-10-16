all:	semaphore semaphoreI

semaphore:	semaphore_example.c
	gcc -Wall semaphore_example.c -o semaphore -lpthread

semaphoreI:	semaphore_exampleImproved.c
	gcc -Wall semaphore_exampleImproved.c -o semaphoreI -lpthread

clean:
	rm -f semaphore semaphoreI
