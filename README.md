# Phasmophobia Simulator
## What is Phasmophobia?
"Phasmophobia is a 4 player online co-op psychological horror. Paranormal activity is on the rise and it’s up to you and your team to use all the ghost-hunting equipment at your disposal in order to gather as much evidence as you can." - Steam
## Overview

Phasmophobia is a simulation program that allows users to input names for four hunters and simulates ghost hunting scenarios. It includes several files for compilation and execution.

## Files

- `defs.h`
- `ghost.c`
- `house.c`
- `hunter.c`
- `logger.c`
- `main.c`
- `makefile`
- `room.c`
- `utils.c`
- `README.txt`

## Compiling

To compile the program, run the following command in the terminal:

```bash
make 
or 
gcc -Wall -Wextra -o phasmo defs.h ghost.c house.c hunter.c main.c room.c utils.c logger.c -lpthread
```
## Running
To run the program, execute the following command in the terminal:
```bash
./phasmo
```
## Cleaning
To clean the project directory and remove object and executable files, use the following command:
```bash
make clean
```
## Usage
Once the program is running, you will be prompted to input the names for the four hunters. Afterward, the simulation results will be displayed on the screen.
