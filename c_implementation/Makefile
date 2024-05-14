all: mini_ml
mini_ml:
	gcc -std=c99 -Wall -fsanitize=address,undefined -o mini_ml *.c -lm -lcsfml-graphics -lcsfml-system -lcsfml-window
.PHONY:
	clean all
clean:
	rm mini_ml
