#include <stdlib.h>
#include <ncurses.h>

#include "constants.h"
#include "canvas.h"
#include "utils.h"

/* TODO: put a list of charts and plot functions to be able to plot multiple charts on a single plot */
struct Plot {
	PlotStatus (*plot_function)(void *, struct Dimentions *, char *, Color *);
	Canvas *canvas;
	void *chart;
};

char *
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
		case ERR_LINE_OUT:
			err_msg = "Line chart doesn't fit on canvas with that scale, consider shrinking it";
			break;
		default:
			err_msg = "Error";
	}

	return err_msg;
}

void
execute_plot(struct Plot *plot)
{
	char *err_msg;
	PlotStatus status = plot->plot_function(
		plot->chart,
		canvas_get_dimentions(plot->canvas),
		canvas_get_canvas(plot->canvas),
		canvas_get_colors_fg(plot->canvas)
	);

	/* TODO: Make error message pop in a nice way, like the prompt_get_note from anote */
	if (status != PLOT_OK) {
		err_msg = rotten_tarts(status);
		canvas_set_scale(plot->canvas, canvas_get_scale(plot->canvas) - SCALE_INCREMENT);
	}
	(void)err_msg;
}

struct Plot *
new_plot(Canvas *canvas)
{
	struct Plot *p = malloc(sizeof(struct Plot));
	p->canvas = canvas;
	return p;
}

void
plot_setup(struct Plot *p, PlotStatus (*function)(void *, struct Dimentions *, char *, Color *), void *chart)
{
	p->plot_function = function;
	p->chart = chart;
}

void
plot_set_function(struct Plot *p, PlotStatus (*function)(void *, struct Dimentions *, char *, Color *))
{
	p->plot_function = function;
}

void
plot_set_canvas(struct Plot *p, Canvas *canvas)
{
	p->canvas = canvas;
}

void
plot_set_chart(struct Plot *p, void *chart)
{
	p->chart = chart;
}
