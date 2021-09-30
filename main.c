/* INCLUDES */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>

#include "canvas.h"
#include "constants.h"
#include "pie_chart.h"
#include "bar_chart.h"
#include "line_chart.h"
#include "utils.h"

int max_height;
int max_width;

/* exits in case a chart can't be plotted */
void
rotten_tarts(PlotStatus status)
{
	char *err_msg;
	switch (status) {
		case PLOT_OK:
			//OK status but error caught, something weird happened
			err_msg = "Something weird happened, exiting.";
			break;
		case ERR_CIRCLE_TOO_BIG:
			err_msg = "Pie chart doesn't fit on canvas with that scale, consider shrinking it";
			break;
		default:
			err_msg = "Error";
	}
	fprintf(stderr, "%s\n", err_msg);
	exit(status);
}

void
show_cursor(int show)
{
	if (show)
		curs_set(1);
	else
		curs_set(0);
}

WINDOW *
create_new_win(int height, int width, int start_y, int start_x)
{
	WINDOW *local_win;

	local_win = newwin(height, width, start_y, start_x);

	/* use default caracters for borders */
	box(local_win, 0 , 0);
	wrefresh(local_win);

	return local_win;
}

void
init_tart(void)
{
	/* start ncurses     */
	if (initscr() == NULL) {
		perror(strerror(errno));
		exit(errno);
	}

	raw();
	noecho();
	show_cursor(0);
	getmaxyx(stdscr,max_height,max_width);
}

void
housekeeping(Canvas *s, WINDOW *w)
{
	delwin(w);
	destroy_canvas(s);
	show_cursor(1);
	echo();
	endwin();
}

int
main()
{
	init_tart();

	int c;
	PlotStatus status;
	int x_offset = 2;
	float scale = (float)max_height / (float)max_height;
	Bar *b1 = new_bar(25, "lucca");
	Bar *b2 = new_bar(5, "luquinha");
	Bar *b3 = new_bar(50, "luccão");
	Pie *p = new_pie(10,max_height/2, 14, 50);
	float series[2] = {2.5, 22.0};
	Canvas *canvas = new_canvas(max_height-1, max_width-1);
	Line *l = new_line(series, "teste", canvas_get_width(canvas), 2);
	WINDOW *tarts_w = create_new_win(max_height, max_width, 0, 0);

	(void)b1;
	(void)b2;
	(void)b3;
	(void)p;
	(void)x_offset;
	(void)l;

	do {
		canvas_clear(canvas);

		/*
		print_bar(b1, canvas, x_offset, scale);
		x_offset += (strlen(bar_get_name(b1)) + 1);

		print_bar(b2, canvas, x_offset, scale);
		x_offset += (strlen(bar_get_name(b2)) + 1);

		print_bar(b3, canvas, x_offset, scale);

		if (x_offset >= max_width)
			x_offset = 2;
		   */

		print_line_chart(l, canvas, scale);
		if ((status = print_pie(p, canvas, scale)))
			rotten_tarts(status);

		show_canvas(canvas, tarts_w);
		wrefresh(tarts_w);
		doupdate();
	} while ((c = getchar()) != 'q');
	housekeeping(canvas, tarts_w);

	return 0;
}
