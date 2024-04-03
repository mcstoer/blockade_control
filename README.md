# Blockade Control

A board game for UVic's SENG 475 final project.

## Game Information

### Game Rules:
1. Each player can place up to a full square worth of pieces each turn.
2. When placing a piece, it needs to be touching a corner of one of the same player's pieces.
3. When placing a piece, it cannot overlap another piece except for on just the corner.
4. The score of a player is determined by how many pieces they can potentially place on the board once the game ends.

### Game End and Scoring:
1. The game automatically ends once no moves can be made that could increase a player's score.
2. When the game automatically ends, it places all the possible pieces for each player on the board. The pieces on the board in this state are the score of each player.
3. The score is printed out to the command line after the game is completed.

## Dependencies

This project needs C++20 and at minimum CMake 3.26.3.

The libraries needed are
- GLFW 3 for the graphical interface and inputs
- GLU for translating mouse controls
- OpenGL for the graphical interface
- Catch2 v2 for tests

## Building

From the root of the source tree, run

```
cmake -H. -Bbuild
cmake --build build
``` 

In the build directory there will be the `blockadecontrol` executable for the game.

## Installing

The game can also be installed into the `bin` directory. 

From the root of the source tree, run
```
cmake --build build --target install
```

## Using

To start the game, run the `blockadecontrol` exectuable.

### Controls:
- Use the mouse to move the piece.
- The right mouse button switches the piece between a triangle, square and rectangle.
- Scrolling rotates the piece.
- The left mouse button places the piece.
- `ESC` exits the application.


