all:
	gcc -Wall -Wextra -g src/main.c src/boiler.c src/logic.c -o game -lSDL3 -lSDL3_image -lSDL3_ttf

clean:
	rm -f game
