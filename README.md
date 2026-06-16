# snake_push_block

A snake game with block-pushing mechanics, built with C and SDL3.

A noice hand crafted sloppy code by yours truly...

## images

<img width="1512" height="914" alt="imag4" src="https://github.com/user-attachments/assets/9f7def38-4062-45dd-b852-dfa658a9ecd2" />
<img width="1512" height="946" alt="image1" src="https://github.com/user-attachments/assets/2f11d1e2-8c21-405b-a761-305cc9d83246" />
<img width="1919" height="1052" alt="Screenshot 2026-06-16 at 5 56 39 PM" src="https://github.com/user-attachments/assets/ba25bab1-0b5b-4c12-905b-08ab12613506" />


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

## AI USE

- I used AI to help make a release for MacOs with Claude code.
- It was also late before the deadline so I used Claude code to help with level 4. (I am sure
  you can tell 😭)

- everything else was done by me! (first time using SDL3, was fun!)
