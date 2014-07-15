/*
 * tic-tac-toe
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _TTT_H
#define _TTT_H

#include <ncurses.h>
#include <math.h>

#define PLAYER1_STRING  "  PLAYER 1  "
#define PLAYER2_STRING  "  PLAYER 2  "
#define YOU_STRING      "    YOU     "
#define COMPUTER_STRING "  COMPUTER  "

#define SWITCH_PLAYER -1

typedef enum _ElementType ElementType;
typedef enum _PlayerType PlayerType;
typedef enum _GameType GameType;
typedef enum _GameState GameState;
typedef struct _Game Game;
typedef struct _Player Player;
typedef struct _Grid Grid;
typedef struct _ttt ttt;

enum _ElementType {
	ELEMENT_TYPE_X = 0,
	ELEMENT_TYPE_O,
	ELEMENT_TYPE_NONE,
	NUM_ELEMENTS,
};

enum _PlayerType {
        PLAYER_TYPE_COMPUTER = 1,
	PLAYER_TYPE_PLAYER1 = -1,
	PLAYER_TYPE_PLAYER2 = -2,
	PLAYER_TYPE_NONE = 0
};

enum _GameType {
	SINGLE_PLAYER, /* player vs computer */
	TWO_PLAYER,    /* player vs player   */
	GAME_TYPE_NONE
};

enum _GameState {
	GAME_STATE_COMPLETE,
	GAME_STATE_INCOMPLETE,
        GAME_STATE_DRAW,
};

struct _Player {
	PlayerType player;
	ElementType element;
	char *player_str;
};

struct _Game {
	GameType game_type;
	Player player[2];
	Player *cur_player;
	GameState game_state;
};

struct _Grid{
	WINDOW *element;
	ElementType type;
};

struct _ttt {
	const char *title;
	WINDOW *board;
	WINDOW *player;
	WINDOW *p[2];
	WINDOW *msg;
	Grid grid[3][3];
        int arr[9]; /* Mapping of grid to an array */
        int freecells;
	Game *game;
	int cur_x;
	int cur_y;
};

#define DEG_TO_RAD(deg) (deg * (180.0f/M_PI))

#define ELEMENT_HT 9
#define ELEMENT_WD 20

/* Keys */
#define KEY_ESC 27

#endif /* _TTT_H */
