all: programa

programa: tablahash.o intervalo.o itree.o
	gcc -Wall -Wextra -Werror -g -std=c99 -o interprete interprete.c tablahash.o intervalo.o itree.o

tablahash.o: ./TablaHash/tablahash.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./TablaHash/tablahash.c

intervalo.o: ./Intervalo/intervalo.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./Intervalo/intervalo.c

itree.o: ./ITree/itree.c
	gcc -Wall -Wextra -Werror -g -std=c99 -c ./ITree/itree.c

clean:
	rm -f programa *.o
