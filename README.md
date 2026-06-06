# snake_push_block

A snake game with block-pushing mechanics, built with C and SDL3.


## Build from Source

### Dependencies

- GCC
- [SDL3](https://github.com/libsdl-org/SDL)

#### macOS

```sh
brew install sdl3
```

On Apple Silicon Macs, you may need to add these to your `~/.zshrc` so the compiler can find Homebrew libraries:

```sh
export CPATH=/opt/homebrew/include:$CPATH
export LIBRARY_PATH=/opt/homebrew/lib:$LIBRARY_PATH
```

#### Linux (Debian/Ubuntu)

```sh
sudo apt install libsdl3-dev
```

#### Windows

Download SDL3 development libraries from the [SDL releases page](https://github.com/libsdl-org/SDL/releases) and add them to your compiler's include/lib paths.

## Build & Run

```sh
make
./game
```

## Controls

Coming soon.
