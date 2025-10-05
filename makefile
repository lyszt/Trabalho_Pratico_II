all: clean
	gcc main.c sorting.c -o main

clean:
	rm -f main