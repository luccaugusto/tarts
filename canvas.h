#ifndef CANVAS_H
#define CANVAS_H

#define BLANK ' '
#define SCALE_BLOCK '-'

typedef struct Canvas Canvas;

void canvas_set_width(struct Canvas *c, int width);
void canvas_set_height(struct Canvas *c, int height);
void canvas_set_scale(struct Canvas *s, double scale);
void canvas_set_show_scale(struct Canvas *s, int show_scale);
int canvas_get_width(struct Canvas *c);
int canvas_get_height(struct Canvas *c);
int canvas_get_size(struct Canvas *c);
int canvas_get_show_scale(struct Canvas *s);
struct Dimentions *canvas_get_dimentions(struct Canvas *s);
double canvas_get_scale(struct Canvas *s);
char *canvas_get_canvas(struct Canvas *c);
Color *canvas_get_colors_bg(struct Canvas *s);
Color *canvas_get_colors_fg(struct Canvas *s);
void destroy_canvas(struct Canvas *c);
void print_canvas(struct Canvas *c);
void show_scale(struct Canvas *c);
void show_canvas(struct Canvas *c, WINDOW *w);
void canvas_clear(struct Canvas *c);
struct Canvas *new_canvas(int height, int width);
void init_colorpairs(void);
int colornum(int fg, int bg);


#endif
