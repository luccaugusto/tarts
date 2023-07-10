#ifndef PLOT_H
#define PLOT_H


#define HEADER_HEIGHT 3

#define CLEAR_WINDOW(x)                                      \
	wmove(x, HEADER_HEIGHT, 1);                              \
wclrtobot(x);

typedef struct Tart Tart;

void show_commands_panel();
PlotStatus bake(Tart *tart);
void destroy_tart(struct Tart *t);
Canvas *tart_get_canvas(struct Tart *t);
int tart_get_chart_count(struct Tart *t);
Tart *new_tart(Canvas *canvas, WINDOW *w);
void tart_set_window(struct Tart *t, WINDOW *w);
void tart_set_canvas(struct Tart *t, Canvas *canvas);
int tart_add_chart(struct Tart *t, void *chart, PlotFunction tart_function, ChartType type);
void execution_loop(Tart *tart, WINDOW *tarts_w, WINDOW *footer_w, double scale);

#endif
