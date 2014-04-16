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

typedef enum _ElementType ElementType;
typedef enum _PlayerType PlayerType;
typedef enum _GameType GameType;
typedef enum _GameState GameState;
typedef struct _Grid Grid;
typedef struct _ttt ttt;

enum _ElementType {
	ELEMENT_TYPE_X,
	ELEMENT_TYPE_O,
	ELEMENT_TYPE_NONE,
	NUM_ELEMENTS,
};

enum _PlayerType {
	PLAYER_TYPE_PLAYER1,
	PLAYER_TYPE_PLAYER2,
	PLAYER_TYPE_COMPUTER,
	PLAYER_TYPE_NONE
};

enum _GameType {
	SINGLE_PLAYER, /* player vs computer */
	TWO_PLAYER,    /* player vs player   */
	GAME_TYPE_NONE
};

enum _GameState {
	GAME_STATE_COMPLETE,
	GAME_STATE_INCOMPLETE,
};
struct _Grid{
	WINDOW *element;
	ElementType type;
	int xoff;
	int yoff;
};

struct _ttt {
	const char *title;
	WINDOW *board;
	WINDOW *player;
	WINDOW *p[2];
	WINDOW *msg;
	Grid grid[3][3];
	int cur_x;
	int cur_y;
	GameType game_type;
	PlayerType cur_player;
};

#define DEG_TO_RAD(deg) (deg * (180.0f/M_PI))

#define ELEMENT_HT 9
#define ELEMENT_WD 20

/* Keys */
#define KEY_ESC 27

#endif /* _TTT_H */
