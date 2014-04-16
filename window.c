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

#include "window.h"

/*
 * Set window to attribute 'attr'
 */
static void _window_attr_clear(WINDOW * win, int height, int width, chtype attr)
{
        int i, j;

        wattrset(win, attr);

        for (i = 0; i < height; i++) {
                wmove(win, i, 0);
                for (j = 0; j < width; j++)
                        waddch(win, ' ');
        }

        touchwin(win);
}

void window_draw_box(WINDOW *win,
		     int y, int x,
		     int height, int width,
		     chtype box, chtype border)
{
	int i, j;

	for (i = 0; i < height; i++) {
		wmove(win, y + i, x);
		for (j = 0; j < width - 2; j++)
			if (!i && !j)
				waddch(win, border | ACS_ULCORNER);
			else if (i == height - 1 && !j)
				waddch(win, border | ACS_LLCORNER);
			else if (!i && j == width - 3)
				waddch(win, box | ACS_URCORNER);
			else if (i == height - 1 && j == width - 3)
				waddch(win, box | ACS_LRCORNER);
			else if (!i)
				waddch(win, border | ACS_HLINE);
			else if (i == height - 1)
				waddch(win, box | ACS_HLINE);
			else if (!j)
				waddch(win, border | ACS_VLINE);
			else if (j == width - 3)
				waddch(win, box | ACS_VLINE);
			else
				waddch(win, box | ' ');
	}
}

void window_draw_edge(WINDOW *win,
		      int y, int x,
		      int height, int width,
		      chtype box, chtype border)
{
	int i, j;

	for (i = 0; i < height; i++) {
		wmove(win, y + i, x);
		for (j = 0; j < width - 2; j++)
			if (!i && !j)
				waddch(win, border | ACS_ULCORNER);
			else if (i == height - 1 && !j)
				waddch(win, border | ACS_LLCORNER);
			else if (!i && j == width - 3)
				waddch(win, box | ACS_URCORNER);
			else if (i == height - 1 && j == width - 3)
				waddch(win, box | ACS_LRCORNER);
			else
				waddch(win, box | ' ');
	}
}

void window_clear(ttt *t)
{
	_window_attr_clear(stdscr, LINES, COLS, A_NORMAL);

        if (t->title != NULL) {
                int i;

                wattrset(stdscr, A_BOLD);
                mvwaddstr(stdscr, 0, 1, (char *)t->title);
                wmove(stdscr, 1, 1);
                for (i = 1; i < COLS - 1; i++)
                        waddch(stdscr, ACS_HLINE);
        }
        wnoutrefresh(stdscr);
}

