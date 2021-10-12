#include <stdlib.h>
#include <ncurses.h>

#include "constants.h"
#include "canvas.h"

struct Plot {
	PlotStatus (*plot_function)(void *, struct Dimentions *, char *, Color *);
	Canvas *canvas;
	void *chart;
};

PlotStatus
execute_plot(struct Plot *plot)
{
	PlotStatus status = PLOT_OK;
	plot->plot_function(
		plot->chart,
		canvas_get_dimentions(plot->canvas),
		canvas_get_canvas(plot->canvas),
		canvas_get_colors_fg(plot->canvas)
	);
	return status;
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
