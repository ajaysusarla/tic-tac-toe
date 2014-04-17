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

#ifndef DRAW_H
#define DRAW_H

#include <ncurses.h>
#include "ttt.h"

void draw_o(Grid *element);
void draw_x(Grid *element);
void draw_grid(ttt *t);
void redraw_grid(ttt *t,
		 int selected_x,
		 int selected_y);
void draw_player_boxes(ttt *t, int user);
void draw_board(ttt *t);

#endif /* DRAW_H */
