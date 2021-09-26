#ifndef CANVAS_H
#define CANVAS_H

typedef struct Screen Screen;

void canvas_set_width(struct Canvas *c, int width);
void canvas_set_height(struct Canvas *c, int height);
int canvas_get_width(struct Canvas *c);
int canvas_get_height(struct Canvas *c);
int canvas_get_size(struct Canvas *c);
char *canvas_get_canvas(struct Canvas *c);
void destroy_canvas(struct Canvas *c);
void show_canvas(struct Canvas *c, WINDOW *w);
void canvas_clear(struct Canvas *c);
struct Canvas *new_canvas(int height, int width);

#endif
