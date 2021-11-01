#ifndef TARTS_UI_H
#define TARTS_UI_H

#define HEADER_HEIGHT 3

#define CLEAR_WINDOW(x)                                      \
	wmove(x, HEADER_HEIGHT, 1);                              \
	wclrtobot(x);

void execution_loop(Tart *tart, WINDOW *tarts_w, WINDOW *footer_w, double scale);
void show_commands_panel();

#endif
