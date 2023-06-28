# Chip 8 Emulator / Virtual Machine
![vm workflow linux](https://github.com/ColleagueRiley/c8/actions/workflows/linux.yml/badge.svg)
![vm workflow windows](https://github.com/ColleagueRiley/c8/actions/workflows/windows.yml/badge.svg)

This [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) emulator is built off of an edited version of [this repo](https://github.com/scanlong/c8/)

# pong.ch8
the pong.ch8 rom comes from [this repo](https://github.com/kripod/chip8-roms/tree/master)

# building 

## Linux
simply run `make buildLinux`

or\
`gcc source/main.c -I include -lX11 -o RVM.exe`

## Windows
simply run `make buildWindows`

or\
`gcc source/main.c -I include -lgdi -lshell32 -o RVM.exe`


then to run the VM simply run `RVM` or `RVM.exe` as per OS