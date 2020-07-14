all: programa

programa: tablahash.o interval.o itree.o
	gcc -Wall -Wextra -Werror -g -std=c99 -o interprete interprete.c tablahash.o interval.o itree.o

tablahash.o: ./TablaHash/tablahash.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./TablaHash/tablahash.c

interval.o: ./Interval/interval.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./Interval/interval.c

itree.o: ./ITree/itree.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./ITree/itree.c

clean:
	rm -f programa *.o
