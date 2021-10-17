#include <stdlib.h>
#include <ncurses.h>

#include "./utils.h"
#include "./constants.h"
#include "./canvas.h"

struct ChartFunctionTuple {
	void *chart;
	PlotFunction plot_function;
};

struct Plot {
	int chart_count;
	struct ChartFunctionTuple chart_list[MAX_CHARTS];
	Canvas *canvas;
};

char *
rotten_tarts(PlotStatus status)
{
	char *err_msg;
	switch (status) {
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
execute_plot(struct Plot *plot, WINDOW *w)
{
	char *err_msg;
	(void)err_msg;
	canvas_clear(plot->canvas);

	if (plot->chart_count == 0) {
		mvwprintw(
			w,
			canvas_get_height(plot->canvas)/2,
			canvas_get_width(plot->canvas)/4,
			"No chart specified"
		);
		return;
	}

	for (int i=0; i<plot->chart_count; ++i) {
		PlotStatus status = plot->chart_list[i].plot_function(
				plot->chart_list[i].chart,
				canvas_get_dimentions(plot->canvas),
				canvas_get_canvas(plot->canvas),
				canvas_get_colors_fg(plot->canvas)
				);

		/* TODO: Make error message pop in a nice way, like the prompt_get_note from anote */
		if (status != PLOT_OK) {
			err_msg = rotten_tarts(status);
			canvas_set_scale(plot->canvas, canvas_get_scale(plot->canvas) - SCALE_INCREMENT);
			break;
		}
	}
	show_canvas(plot->canvas, w);
}

struct Plot *
new_plot(Canvas *canvas)
{
	struct Plot *p = malloc(sizeof(struct Plot));
	p->canvas = canvas;
	return p;
}

void
plot_set_canvas(struct Plot *p, Canvas *canvas)
{
	p->canvas = canvas;
}

int
plot_add_chart(struct Plot *p, void *chart, PlotFunction plot_function)
{
	if (p->chart_count >= MAX_CHARTS)
		return 0;

	struct ChartFunctionTuple cft;
	cft.chart = chart;
	cft.plot_function = plot_function;

	p->chart_list[p->chart_count++] = cft;
	return 1;
}

void
destroy_plot(struct Plot *p)
{
	for (int i=0; i<p->chart_count; ++i) {
	}
	free(p);
}
