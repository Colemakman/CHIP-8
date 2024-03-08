# CHIP-8 Emulator

Emulator Written in C and SDL that runs CHIP-8 games.  
CHIP-8: https://en.wikipedia.org/wiki/CHIP-8  

Run with `-d` or `--debug` flags to view registers and iterate instructions one by one. 

### Installation

## Linux

##### Clone the repo
```bash
git clone https://github.com/Colemakman/CHIP-8/
```
##### Install dependancies

```bash
sudo apt install gcc libsdl2-dev
```
##### Build

```bash
make
```
##### Run

```bash
./chip8 example.ch8
```

## Windows

Do the same steps but on WSL.
https://learn.microsoft.com/en-us/windows/wsl/install
You may need to `sudo apt install make ncurses-dev`

