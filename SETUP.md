# SETUP — building the 2D game on Windows

Notes for getting a clean build from scratch. Written for **Windows 11 + MSYS2 UCRT64**.

## 1. What had to be downloaded

### MSYS2 itself
- Installer from <https://www.msys2.org/> (the `msys2-x86_64-*.exe`).
- MSYS2 ships **several** terminals (MSYS, MINGW64, UCRT64, ...). We use the
  **UCRT64** one. Always open the terminal titled **"MSYS2 UCRT64"** — the others
  use a different compiler/runtime and the packages below won't be on their PATH.

### Packages (installed inside the UCRT64 terminal with `pacman`)
The UCRT64 versions of each — note the `mingw-w64-ucrt-x86_64-` prefix:

| What | Package |
|------|---------|
| C compiler (gcc) | `mingw-w64-ucrt-x86_64-gcc` |
| SDL2 (core library) | `mingw-w64-ucrt-x86_64-SDL2` |
| SDL2_image (sprites, later) | `mingw-w64-ucrt-x86_64-SDL2_image` |
| pkg-config replacement | `mingw-w64-ucrt-x86_64-pkgconf` |

Install command (one line, run in UCRT64):

```
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-pkgconf
```

Good habit before installing anything: `pacman -Syu` to sync/update the package
database (it may ask you to close and reopen the terminal the first time).

### Verify the tools are found
```
gcc --version            # should report gcc 16.1.0 (or newer)
pkg-config --modversion sdl2
```
If either "command not found", you're probably in the wrong MSYS2 terminal
(not UCRT64) or need to reopen it after installing.

## 2. How to compile

Run all commands **from the project root** (`C:\Users\halfs\personalProjects\2DGame`)
in the **UCRT64** terminal.

### The flags come from pkg-config
`pkg-config --cflags --libs sdl2` prints the compiler/linker flags SDL needs.
It expands to something like:

```
-Dmain=SDL_main -I/ucrt64/include/SDL2 -Dmain=SDL_main -L/ucrt64/lib -lmingw32 -lSDL2main -lSDL2
```

You don't type those by hand — you let the shell substitute them with `$(...)`.

### Build command
Because the project now has **more than one `.c` file**, list them all:

```
gcc src/main.c src/player.c -o game.exe $(pkg-config --cflags --libs sdl2)
```

- Add every new `.c` file to that list as you create them. (`.h` headers are
  NOT listed — they get pulled in by `#include`.)
- `-o game.exe` names the output. Drop it and gcc defaults to `a.exe`.
- Put the `$(pkg-config ...)` at the **end**, after your source files. Link
  order matters in gcc.

### Run it
```
./game.exe
```

## 3. The one gotcha to remember: `-Dmain=SDL_main`

Notice `pkg-config` emits `-Dmain=SDL_main`. On Windows, SDL hijacks `main` so it
can do its own setup first. For the link to succeed your entry point MUST be
declared exactly:

```
int main(int argc, char *argv[])
```

If you write `int main(void)` or `void main()`, you'll get a confusing linker
error (an "undefined reference to `SDL_main`" or similar). If that happens, this
is the first thing to check.

## 4. Quick reference

```
# one-time setup (UCRT64 terminal)
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-pkgconf

# every build (from project root, UCRT64)
gcc src/main.c src/player.c -o game.exe $(pkg-config --cflags --libs sdl2)
./game.exe
```
