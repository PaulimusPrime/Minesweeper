# Minesweeper
## Project description
One of the assignements in TDT4102 - Procedural and Object-Oriented Programming at NTNU
Using libraries provided by NTNU. 

This project aims to produce the graphics and functionality required to play the classical game minesweeper.

## How to play
- `control + fn + F5` to run the game.
- Open a tile with left click.
- Flag a tile with right click.
- Flag all mine-tiles to win the game!

<img src="GameScreen.png" alt="drawing" width="300"/>
 
## Editability
The game can be edited in ```main.cpp```:
 ```
int main() {

    constexpr int width = 12;
    constexpr int height = 12;
    constexpr int mines = 12;

    ...
}
```
These values can be chosen to decide whatever wanted size and difficulty of the game.
_Written in VSCode on MacOS_
