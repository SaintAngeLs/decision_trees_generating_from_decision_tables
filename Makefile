all: mini_ml
mini_ml:
	gcc -std=c99 -fsanitize=address,undefined -o mini_ml *.c -lm -lcsfml-system -lcsfml-window -lcsfml-graphics
.PHONY:
	clean all
clean:
	rm mini_ml
