# CHIP-8 Emulator

Emulator Written in C and SDL that runs CHIP-8 games.  
CHIP-8: https://en.wikipedia.org/wiki/CHIP-8  

### Installation

## Linux

##### Clone the repo

`git clone https://github.com/Colemakman/CHIP-8/`

##### Install dependancies

Debian based systems  
`sudo apt install gcc libsdl2-dev`

##### Build

Build emulator  
`make`

Build disassembler  
`make dis`

##### Run

`./chip8 example.ch8`


## Windows

Quite a bit more involved for Windows.

##### Install dependancies

- Install MinGW-w64
- Install SDL2 Development Libraries

##### Set up environment variables

- Add MinGW-w64 bin direcotry to your system's PATH environment variable.
    - Control Panel -> System & Security -> System
    - Advanced system settings -> Properties -> Environment variables
    - Find the 'Path' variable -> Edit
    - Add your path to the MinGW-w64 bin directory (e.g., `C:\MinGW\bin`)

##### Clone the repo

Open cmd or Git Bash
```bash
git clone https://github.com/Colemakman/CHIP-8
```

##### Build

```bash
cd CHIP-8
mingw32-make
```

##### Run
```bash
chip8.exe example.ch8
```

