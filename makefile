all: shell

shell: main.c parse.c batch.c execute.c parse.c interactive.c parse.h
		gcc main.c batch.c parse.c execute.c interactive.c -o shell

clean:
		rm -rf *o shell