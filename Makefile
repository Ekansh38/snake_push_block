VERSION = 1.0.0

all:
	gcc -Wall -Wextra -g src/main.c src/boiler.c src/logic.c -o snake_push_block -lSDL3 -lSDL3_image -lSDL3_ttf

app: all
	VERSION=$(VERSION) ./packaging/macos_bundle.sh

release: app

clean:
	rm -rf snake_push_block snake_push_block.dSYM build

.PHONY: all app release clean
