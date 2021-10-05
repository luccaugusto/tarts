#ifndef CANVAS_H
#define CANVAS_H

#define BLANK ' '

typedef struct Canvas Canvas;

void canvas_set_width(struct Canvas *c, int width);
void canvas_set_height(struct Canvas *c, int height);
int canvas_get_width(struct Canvas *c);
int canvas_get_height(struct Canvas *c);
int canvas_get_size(struct Canvas *c);
char *canvas_get_canvas(struct Canvas *c);
Color *canvas_get_colors_bg(struct Canvas *s);
Color *canvas_get_colors_fg(struct Canvas *s);
void destroy_canvas(struct Canvas *c);
void show_canvas(struct Canvas *c, WINDOW *w);
void canvas_clear(struct Canvas *c);
struct Canvas *new_canvas(int height, int width);
void init_colorpairs(void);

#endif
