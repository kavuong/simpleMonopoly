hoarding.out: main.o monopolyGame.o
	gcc -g -Wall -Werror -o hoarding.out main.o monopolyGame.o

main.o: main.c monopolyGame.h structs.h
	gcc -g -Wall -Werror -c main.c

monopolyGame.o: monopolyGame.c monopolyGame.h structs.h
    gcc -g -Wall -Werror -c monopolyGame.c

clean:
	rm -f *.o *.out