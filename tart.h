#ifndef PLOT_H
#define PLOT_H

typedef struct Tart Tart;

void bake(Tart *tart);
Tart *new_tart(Canvas *canvas, WINDOW *w);
void tart_set_canvas(struct Tart *t, Canvas *canvas);
Canvas *tart_get_canvas(struct Tart *t);
int tart_add_chart(struct Tart *t, void *chart, PlotFunction tart_function);
void destroy_tart(struct Tart *t);
int tart_get_chart_count(struct Tart *t);
void tart_set_window(struct Tart *t, WINDOW *w);

#endif
