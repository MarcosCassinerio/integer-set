all: programa

programa: tablahash.o interval.o set.o
	gcc -Wall -Wextra -Werror -g -std=c99 -lm -o interprete interprete.c interval.o set.o tablahash.o

interval.o: ./Interval/interval.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./Interval/interval.c

set.o: ./Set/set.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./Set/set.c

tablahash.o: ./TablaHash/tablahash.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./TablaHash/tablahash.c

clean:
	rm -f programa *.o
