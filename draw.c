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

#include "draw.h"
#include "window.h"

#define BOARD_WIDTH  60
#define BOARD_HEIGHT 30
#define MSG_BOX_WIDTH 60
#define MSG_BOX_HEIGHT 7
#define PLAYER_BOX_WIDTH 60
#define PLAYER_BOX_HEIGHT 3

void draw_o(Grid *element)
{
	float deg, w, h;
	int y, x;

	wattrset(element->element, A_STANDOUT);
	w = 8.0;
	h = 4.0;

	for (deg = 0; deg < 360.0f; deg += 1.0f) {
		x = w + (int)(w * cos(DEG_TO_RAD(deg)));
		y = h + (int)(h * sin(DEG_TO_RAD(deg)));

		mvwaddch(element->element, y, x, 'O');
	}
}

void draw_x(Grid *element)
{
	int i;
	int xpos, ypos;

	wattrset(element->element, A_STANDOUT);

	for (i = 1; i < (ELEMENT_HT - 1); i++) {
		ypos = i;
		xpos = i*2;
		mvwaddch(element->element, ypos, xpos, 'X');
		mvwaddch(element->element, ypos, xpos+1, 'X');
		mvwaddch(element->element, ypos, ELEMENT_WD-3-xpos, 'X');
		mvwaddch(element->element, ypos, ELEMENT_WD-4-xpos, 'X');
	}

}

void draw_grid(ttt *t)
{
	int i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			t->grid[i][j].element = derwin(t->board,
						       ELEMENT_HT,
						       ELEMENT_WD,
						       i*ELEMENT_HT,
						       j*ELEMENT_WD);
			t->grid[i][j].type = ELEMENT_TYPE_NONE;
			if (i == 0 && j == 0)
				wattrset(t->grid[i][j].element, A_REVERSE);
			window_draw_box(t->grid[i][j].element,
					0, 0, ELEMENT_HT, ELEMENT_WD,
					A_NORMAL, A_NORMAL);
			wnoutrefresh(t->grid[i][j].element);
		}
	}
}

void redraw_grid(ttt *t,
		 int selected_x,
		 int selected_y)
{
	int i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			t->grid[i][j].element = derwin(t->board,
						       ELEMENT_HT,
						       ELEMENT_WD,
						       i*ELEMENT_HT,
						       j*ELEMENT_WD);

			if (i == selected_x && j == selected_y) {
				wattrset(t->grid[i][j].element, A_REVERSE);
			} else {
				wattrset(t->grid[i][j].element, 0);
			}

			window_draw_box(t->grid[i][j].element,
					0, 0, ELEMENT_HT, ELEMENT_WD,
					A_NORMAL, A_NORMAL);

			if (t->grid[i][j].type == ELEMENT_TYPE_X)
				draw_x(&t->grid[i][j]);
			else if (t->grid[i][j].type == ELEMENT_TYPE_O)
				draw_o(&t->grid[i][j]);

			keypad(t->grid[i][j].element, TRUE);
			wnoutrefresh(t->grid[i][j].element);
		}
	}
}

void draw_player_boxes(ttt *t, int user)
{
	int i;

	for (i = 0; i < 2; i++) {
		t->p[i] = derwin(t->player, 3, 30, 0, i*30);
		if (i == user)
			wattrset(t->p[i], A_REVERSE);

		window_draw_edge(t->p[i],
				 0, 0,
				 3, 30,
				 A_NORMAL, A_NORMAL);
		touchwin(t->p[i]);
		wnoutrefresh(t->p[i]);
	}
}

void draw_board(ttt *t)
{
	int xpos, ypos;

	window_clear(t);

	/* the board at the center of the screen */
	xpos = (getmaxx(stdscr) - BOARD_WIDTH) / 2;
	ypos = (getmaxy(stdscr) - BOARD_HEIGHT) / 2;

	t->board = newwin(BOARD_HEIGHT, BOARD_WIDTH, ypos, xpos);
	keypad(t->board, TRUE);

	wattrset(t->board, A_NORMAL);
	wbkgdset(t->board, A_NORMAL & A_COLOR);

	touchwin(t->board);
	wnoutrefresh(t->board);

	/* the grid */
	draw_grid(t);

	/* message box */
	xpos = (getmaxx(stdscr) - MSG_BOX_WIDTH) / 2;
	ypos = (getmaxy(stdscr) - MSG_BOX_HEIGHT);
	t->msg = newwin(MSG_BOX_HEIGHT, MSG_BOX_WIDTH, ypos, xpos);

	window_draw_edge(t->msg,
			 0, 0,
			 MSG_BOX_HEIGHT, MSG_BOX_WIDTH,
			 A_BOLD, A_NORMAL);

	wattrset(t->msg, A_NORMAL);
	wbkgdset(t->msg, A_NORMAL & A_COLOR);

	touchwin(t->msg);
	wnoutrefresh(t->msg);

	/* player box */
	xpos = (getmaxx(stdscr) - PLAYER_BOX_WIDTH) / 2;
	ypos = 2;
	t->player = newwin(PLAYER_BOX_HEIGHT, PLAYER_BOX_WIDTH, ypos, xpos);

	wattrset(t->player, A_NORMAL);
	wbkgdset(t->player, A_NORMAL & A_COLOR);

	/* box */
	draw_player_boxes(t, 0);

	touchwin(t->player);
	wnoutrefresh(t->player);

	wnoutrefresh(t->board);
}
