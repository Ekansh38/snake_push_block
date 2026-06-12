# snake_push_block

A snake game with block-pushing mechanics, built with C and SDL3.


## Build from Source

### Dependencies

- GCC
- [SDL3](https://github.com/libsdl-org/SDL)
- [SDL3_image](https://github.com/libsdl-org/SDL_image)
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)

#### macOS

```sh
brew install sdl3 sdl3_image sdl3_ttf
```

On Apple Silicon Macs, you may need to add these to your `~/.zshrc` so the compiler can find Homebrew libraries:

```sh
export CPATH=/opt/homebrew/include:$CPATH
export LIBRARY_PATH=/opt/homebrew/lib:$LIBRARY_PATH
```

#### Linux (Debian/Ubuntu)

```sh
sudo apt install libsdl3-dev libsdl3-image-dev
```

#### Windows

Download SDL3 development libraries from the [SDL releases page](https://github.com/libsdl-org/SDL/releases) and add them to your compiler's include/lib paths.

## Build & Run

```sh
make
./game
```

## Controls

WASD - to control the snake.
Arrow keys - on homepage to switch level.
Esc - while playing to pause.

Other:

when you collide with a block, press that direction again to push the block.
