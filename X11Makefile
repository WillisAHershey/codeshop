codeshop: main.c fileReps.o userInterface.o
	gcc -o codeshop -O3 -Wall -lX11 main.c fileReps.o userInterface.o

fileReps.o: fileReps.c fileReps.h
	gcc -c -O3 -Wall fileReps.c

userInterface.o: userInterface.c userInterface.h
	gcc -c -O3 -Wall -lX11 userInterface.c

.PHONY: clean

clean:
	rm codeshop fileReps.o userInterface.o
