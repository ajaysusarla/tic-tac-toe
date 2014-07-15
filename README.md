#Tic Tac Toe

This is a curses based implementation of the popular game of Tic Tac Toe. This game features a very simple AI for computing the next move when playing against the computer. 

The core of the AI is losely based on the [minimax algorithm](http://en.wikipedia.org/wiki/Minimax). I specially found the following links helpful in understanding the algorithm:

1. http://www.flyingmachinestudios.com/programming/minimax/
2. http://cs.ucla.edu/~rosen/161/notes/alphabeta.html
 
Although, the second link focuses on [Alpha Beta Pruning](http://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning), and is probably a bit of an overkill for a tic tac toe implementation, it is a good read nevertheless.


Everything begins and ends in the `game_loop()` function in `game.c`. And the only interesting function is `computer_make_move()`, which invokes the `compute_moves()` method, responsible for the finding out the next 'best move' based on the current state of the board. The game board is represented by a `Grid` on screen and as an integer array in memory.

```
t->grid[3][3]                                              t->arr[9]
     ____________________                                      ____
    |      |      |      |                                    |____|
    |      |      |      |                                    |____|   
    |______|______|______|                                    |____|
    |      |      |      |                                    |____|
    |      |      |      |                                    |____|
    |______|______|______|                                    |____|
    |      |      |      |                                    |____|
    |      |      |      |                                    |____|
    |______|______|______|                                    |____|
                                                              |____|
           
   On screen representation.                               In memory representation.
```
