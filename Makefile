all:
	gcc -fopenmp -Wall -Werror -O3 main.c -o proj
clean:
	rm -rf proj