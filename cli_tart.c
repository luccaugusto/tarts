#include <ncurses.h>

#include "./utils.h"
#include "./constants.h"
#include "./canvas.h"
#include "./tart.h"

/* TODO */
void delete_chart(){return;}
void add_new_chart(){return;}
void show_commands_panel(){return;}

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
execution_loop(Tart *tart, WINDOW *tarts_w, WINDOW *footer_w, double scale)
{
	int c = BLANK;
	char *err_msg;
	PlotStatus status = PLOT_OK;
	Canvas *canvas = tart_get_canvas(tart);
	do {
		switch (c) {
			case '?':
				show_commands_panel();
				break;
			case 'i':
				scale += SCALE_INCREMENT;
				canvas_set_scale(canvas, scale);
				break;
			case 'd':
				scale -= SCALE_INCREMENT;
				canvas_set_scale(canvas, scale);
				break;
			case 'n':
				add_new_chart();
				break;
			case 'D':
				delete_chart();
				break;
			default:
				//NOP
				break;
		};
		/* TODO: Make error message pop in a nice way, like the prompt_get_note from anote */
		if ((status = bake(tart)) != PLOT_OK) {
			err_msg = rotten_tarts(status);
			scale -= SCALE_INCREMENT;
			canvas_set_scale(canvas, scale);
		}
		wrefresh(tarts_w);
		wrefresh(footer_w);
		doupdate();
	} while ((c = wgetch(tarts_w)) != 'q');
}
