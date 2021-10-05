/* INCLUDES */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>

#include "constants.h"
#include "canvas.h"
#include "pie_chart.h"
#include "bar_chart.h"
#include "line_chart.h"
#include "utils.h"

int max_height;
int max_width;
char *err_msg = "\0";

/* exits in case a chart can't be plotted */
void
rotten_tarts(PlotStatus status)
{
	switch (status) {
		case PLOT_OK:
			//OK status but error caught, something weird happened
			err_msg = "Something weird happened, exiting.";
			break;
		case ERR_CIRCLE_TOO_BIG:
			err_msg = "Pie chart doesn't fit on canvas with that scale, consider shrinking it";
			break;
		case ERR_LINE_OUT:
			err_msg = "Line chart doesn't fit on canvas with that scale, consider shrinking it";
			break;
		default:
			err_msg = "Error";
	}
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

	 if (has_colors() == FALSE) {
        endwin();
        puts("This program requires a terminal with color support");
        exit(1);
    }

	raw();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	show_cursor(0);
	getmaxyx(stdscr,max_height,max_width);

    start_color();
    init_colorpairs();
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

	int c = BLANK;
	PlotStatus status = PLOT_OK;
	int x_offset = 2;
	float scale = (float)max_height / (float)max_height;
	/*
	*/
	Bar *b1 = new_bar(25, "lucca");
	Bar *b2 = new_bar(5, "luquinha");
	Bar *b3 = new_bar(50, "luccão");
	(void)b1;
	(void)b2;
	(void)b3;
	Pie *p = new_pie(10,max_height/2, 14, 50);
	float series[2] = {2.5, 22.0};
	float series2[4] = {3.5, 17.0, 10.0, 8.9};
	Canvas *canvas = new_canvas(max_height-2, max_width-2);
	//Plot *plot = new_plot(canvas);
	Line *l = new_line(series, "teste", canvas_get_width(canvas), 2);
	Line *l2 = new_line(series2, "teste", canvas_get_width(canvas), 4);
	WINDOW *tarts_w = create_new_win(max_height, max_width, 0, 0);

	(void)p;
	(void)x_offset;
	(void)l;

	do {
		if (c == 'i')
			scale += 0.1;
		else if (c == 'd')
			scale -= 0.1;

		canvas_clear(canvas);

		/*
		plot_setup(plot, &print_bar, b1, scale, 1);
		plot_chart(plot);

		plot_setup(plot, &print_bar, b2, scale, (strlen(bar_get_name(b1)) + 1));
		plot_chart(plot);

		plot_setup(plot, &print_bar, b3, scale, (strlen(bar_get_name(b1)) + 1) + (strlen(bar_get_name(b2)) + 1));
		plot_chart(plot);
		*/

		bar_set_color(b1, COLOR_YELLOW);
		print_bar(b1, canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), canvas_get_colors_fg(canvas), scale);

		if ((status = print_line_chart(l, canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), scale))) {
			rotten_tarts(status);
			break;
		}

		if ((status = print_line_chart(l2, canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), scale))) {
			rotten_tarts(status);
			break;
		}

		if ((status = print_pie(p,  canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), scale))) {
			rotten_tarts(status);
			break;
		}

		show_canvas(canvas, tarts_w);
		wrefresh(tarts_w);
		doupdate();
	} while ((c = getchar()) != 'q');

	housekeeping(canvas, tarts_w);

	if (*err_msg)
		fprintf(stderr, "%s\n", err_msg);

	return status;
}
