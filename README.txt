README

Controls:
-------------
W -> Move piece up
A -> Move piece left
S -> Move piece down
D -> Move piece right

R -> Rotate piece
F -> Switch piece between triangle, square and rectangle
SPACE/ENTER -> Place piece


Game Rules:
-------------
1. Each player can place up to a full square worth of pieces each turn.
2. When placing a piece, it needs to be touching a corner of one of the same player's pieces.
3. When placing a piece, it cannot overlap another piece except for on just the corner.
4. The score of a player is determined by how many pieces they can potentially place 
   on the board once the game ends.


Game Information:
1. The game automatically ends once no moves can be made that could increase a player's score.
2. When the game automatically ends, it places all the possible pieces for each player on 
   the board. The pieces on the board in this state are the score of each player.
3. The score is printed out to the command line after the game is completed.
