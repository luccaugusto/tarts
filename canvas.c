#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "./utils.h"
#include "./constants.h"
#include "./canvas.h"

/* TYPE DEFINITION */
struct Canvas{
	int show_scale;
	char *canvas;
	Color *colors_fg;
	Color *colors_bg;
	struct Dimentions dimentions;
};

/* FUNCTION PROTOTYPES */
Color curs_color(int fg);
int is_bold(int fg);
void unsetcolor(int fg, int bg, WINDOW *w);
void setcolor(int fg, int bg, WINDOW *w);

/* FUNCTION DEFINITION */
void
canvas_zoom(struct Canvas *s, int in_or_out)
{
	double scale = canvas_get_scale(s);
	switch (in_or_out) {
		case ZOOM_IN:
			canvas_set_scale(s, scale + scale * SCALE_STEP);
			break;
		case ZOOM_OUT:
			canvas_set_scale(s, scale - scale * SCALE_STEP);
			break;
	}
}

void
canvas_set_scale(struct Canvas *s, double scale)
{
	if (scale < 0.1) scale = 0.1;
	s->dimentions.scale = scale;
}

void
canvas_set_width(struct Canvas *s, int width)
{
	s->dimentions.width = width;
}

void
canvas_set_height(struct Canvas *s, int height)
{
	s->dimentions.height = height;
}

void
canvas_set_show_scale(struct Canvas *s, int show_scale)
{
	s->show_scale = show_scale;
}

double
canvas_get_scale(struct Canvas *s)
{
	return s->dimentions.scale;
}

int
canvas_get_width(struct Canvas *s)
{
	return s->dimentions.width;
}

int
canvas_get_height(struct Canvas *s)
{
	return s->dimentions.height;
}

int
canvas_get_show_scale(struct Canvas *s)
{
	return s->show_scale;
}

struct Dimentions *
canvas_get_dimentions(struct Canvas *s)
{
	return &s->dimentions;
}

int
canvas_get_size(struct Canvas *s)
{
	return (canvas_get_height(s) * canvas_get_width(s));
}

char *
canvas_get_canvas(struct Canvas *s)
{
	return s->canvas;
}

Color *
canvas_get_colors_bg(struct Canvas *s)
{
	return s->colors_bg;
}

Color *
canvas_get_colors_fg(struct Canvas *s)
{
	return s->colors_fg;
}

void
destroy_canvas(struct Canvas *s)
{
	char *canvas = canvas_get_canvas(s);
	if (canvas != NULL)
		free(canvas);
	free(s);
}

void
print_color(Color color)
{
	switch (color) {
		case COLOR_BLUE:
			printf("%s",CODE_BLUE);
			break;
		case COLOR_GREEN:
			printf("%s",CODE_GREEN);
			break;
		case COLOR_CYAN:
			printf("%s",CODE_CYAN);
			break;
		case COLOR_RED:
			printf("%s",CODE_RED);
			break;
		case COLOR_MAGENTA:
			printf("%s",CODE_MAGENTA);
			break;
		case COLOR_YELLOW:
			printf("%s",CODE_YELLOW);
			break;
		default:
		case COLOR_WHITE:
			printf("%s",CODE_WHITE);
			break;
	}
}

void
print_canvas(struct Canvas *c)
{
	int height = canvas_get_height(c);
	int width = canvas_get_width(c);
	char *canvas = canvas_get_canvas(c);
	Color *colors_fg = canvas_get_colors_fg(c);

	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			print_color(colors_fg[y * width + x]);
			printf("%c%s", canvas[y * width + x],CODE_COLOR_RESET);
		}
		printf("\n");
	}
}

void
show_scale(struct Canvas *c)
{
	char *canvas = canvas_get_canvas(c);
	Color *colors_fg = canvas_get_colors_fg(c);
	double scale = canvas_get_scale(c);
	int height = canvas_get_height(c);
	int width = canvas_get_width(c);
	char *aux = malloc(sizeof(char) * MAX_NAME_LENGTH);

	for (int y=0; y<height; y+=(2 / scale)) {
		for (int x=0; x<width; ++x) {
			canvas[y*width + x] = SCALE_BLOCK;
			colors_fg[y*width + x] = COLOR_WHITE;
		}
		aux = int2str((height- y)/scale);
		strncpy(&canvas[y*width], aux, strlen(aux));
	}
}

void
show_canvas(struct Canvas *c, WINDOW *w)
{
	int height = canvas_get_height(c);
	int width = canvas_get_width(c);
	char *canvas = canvas_get_canvas(c);
	Color *colors_fg = canvas_get_colors_fg(c);
	Color *colors_bg = canvas_get_colors_bg(c);

	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			setcolor(colors_fg[y * width + x], colors_bg[y * width + x], w);
			/* offset printing because of borders */
			mvwprintw(w,y+1, x+1, "%c", canvas[y * width + x] );
			unsetcolor(colors_fg[y * width + x], colors_bg[y * width + x], w);
		}
	}
}

void
canvas_clear(struct Canvas *s)
{
	char *c = s->canvas;
	int n = canvas_get_height(s) * canvas_get_width(s) - 1;
	int nfg = n;
	int nbg = n;
	while (n >= 0) c[n--] = BLANK;
	while (nfg >= 0) s->colors_fg[nfg--] = COLOR_BLANK;
	while (nbg >= 0) s->colors_fg[nbg--] = COLOR_BLANK;
}

struct Canvas *
new_canvas(int height, int width)
{
	struct Canvas *s = malloc(sizeof(struct Canvas));
	canvas_set_height(s, height);
	canvas_set_width(s, width);
	canvas_set_scale(s, 1);
	s->canvas = malloc(sizeof(char) * (height) * (width));
	s->colors_fg = malloc(sizeof(int) * (height) * (width));
	s->colors_bg = malloc(sizeof(int) * (height) * (width));
	s->show_scale = 0;

	canvas_clear(s);

	return s;
}

/* Color functions taken from
 * https://www.linuxjournal.com/content/about-ncurses-colors-0
 */
Color
curs_color(int fg)
{
	Color color;

	switch (7 & fg) {
		case 0:
			color = COLOR_BLACK;
			break;
		case 1:
			color = COLOR_BLUE;
			break;
		case 2:
			color = COLOR_GREEN;
			break;
		case 3:
			color = COLOR_CYAN;
			break;
		case 4:
			color = COLOR_RED;
			break;
		case 5:
			color = COLOR_MAGENTA;
			break;
		case 6:
			color = COLOR_YELLOW;
			break;
		case 7:
			color = COLOR_WHITE;
			break;
		default:
			color = COLOR_BLACK;
	}

	return color;
}

/* return the intensity bit */
int
is_bold(int fg)
{
    int i;
    i = 1 << 3;
    return (i & fg);
}

int
colornum(int fg, int bg)
{
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}

/* set the color pair (colornum) and bold/bright (A_BOLD) */
void
setcolor(int fg, int bg, WINDOW *w)
{
	wattrset(w, COLOR_PAIR(colornum(fg, bg)));
	if (is_bold(fg)) {
		wattrset(w, A_BOLD);
	}
}

/* unset the color pair (colornum) and
   bold/bright (A_BOLD) */
void
unsetcolor(int fg, int bg, WINDOW *w)
{
	wattroff(w, COLOR_PAIR(colornum(fg, bg)));
	if (is_bold(fg)) {
		wattroff(w, A_BOLD);
	}
}

void
init_colorpairs(void)
{
	int fg, bg;
	int colorpair;

	for (bg = 0; bg <= 7; bg++) {
		for (fg = 0; fg <= 7; fg++) {
			colorpair = colornum(fg, bg);
			init_pair(colorpair, curs_color(fg), curs_color(bg));
		}
	}
}
