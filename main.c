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
double scale;
char *err_msg = "\0";
WINDOW *tarts_w;

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
			scale -= SCALE_INCREMENT;
			break;
		case ERR_LINE_OUT:
			err_msg = "Line chart doesn't fit on canvas with that scale, consider shrinking it";
			scale -= SCALE_INCREMENT;
			break;
		default:
			err_msg = "Error";
	}

	mvwprintw(tarts_w, max_height/2 - 1, max_width/4,"-------------------------------------------");
	mvwprintw(tarts_w, max_height/2, max_width/4,"%s [press any key to continue]", err_msg);
	mvwprintw(tarts_w, max_height/2 + 1, max_width/4,"-------------------------------------------");
	getchar();
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
housekeeping(Canvas *s)
{
	delwin(tarts_w);
	//destroy_canvas(s);
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
	scale = (double)max_height / (double)max_height;

	Bar *b1 = new_bar(25, "lucca");
	bar_set_color(b1, COLOR_YELLOW);
	Bar *b2 = new_bar(5, "luquinha");
	Bar *b3 = new_bar(50, "luccão");
	(void)b1;
	(void)b2;
	(void)b3;
	Pie *p = new_pie(10,max_height/2, 14, 50);
	Portion *portion_1 = new_portion(50, "metade 1", COLOR_GREEN);
	Portion *portion_2 = new_portion(50, "metade 2", COLOR_RED);
	pie_push_portion(p, portion_1);
	pie_push_portion(p, portion_2);
	/*
	*/

	double series[2] = {2.5, 22.0};
	double series2[4] = {3.5, 17.0, 10.0, 8.9};
	Canvas *canvas = new_canvas(max_height-2, max_width-2);
	//Plot *plot = new_plot(canvas);
	Line *l = new_line(series, "teste", canvas_get_width(canvas), 2);
	line_set_color(l, COLOR_BLUE);
	Line *l2 = new_line(series2, "teste", canvas_get_width(canvas), 4);
	line_set_color(l2, COLOR_GREEN);

	tarts_w = create_new_win(max_height, max_width, 0, 0);

	(void)p;
	(void)x_offset;
	(void)l;

	do {
		if (c == 'i')
			scale += SCALE_INCREMENT;
		else if (c == 'd')
			scale -= SCALE_INCREMENT;

		canvas_clear(canvas);

		/*
		plot_setup(plot, &print_bar_chart, b1, scale, 1);
		plot_chart(plot);

		plot_setup(plot, &print_bar_chart, b2, scale, (strlen(bar_get_name(b1)) + 1));
		plot_chart(plot);

		plot_setup(plot, &print_bar_chart, b3, scale, (strlen(bar_get_name(b1)) + 1) + (strlen(bar_get_name(b2)) + 1));
		plot_chart(plot);

		//print_bar_chart(b1, canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), canvas_get_colors_fg(canvas), scale);

		if ((status = print_line_chart(l2, canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), canvas_get_colors_fg(canvas), scale))) {
			rotten_tarts(status);
		}

		if ((status = print_line_chart(l, canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), canvas_get_colors_fg(canvas), scale))) {
			rotten_tarts(status);
		}
		*/

		/*
		if ((status = print_pie(p,  canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), canvas_get_colors_fg(canvas), scale))) {
			rotten_tarts(status);
		}
		*/

		if ((status = print_pie(p,  canvas_get_width(canvas), canvas_get_height(canvas), canvas_get_canvas(canvas), canvas_get_colors_fg(canvas), scale))) {
		rotten_tarts(status);
		}

		show_canvas(canvas, tarts_w);
		wrefresh(tarts_w);
		doupdate();
	} while ((c = getchar()) != 'q');
	housekeeping(canvas);

	return status;
}
