#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "./constants.h"

/* TYPE DEFINITION */
struct Canvas{
	int height;
	int width;
	char *canvas;
	CanvasColor *colors_fg;
	CanvasColor *colors_bg;
};

/* FUNCTION PROTOTYPES */
CanvasColor curs_color(int fg);
int is_bold(int fg);
int colornum(int fg, int bg);
void unsetcolor(int fg, int bg, WINDOW *w);
void setcolor(int fg, int bg, WINDOW *w);

/* FUNCTION DEFINITION */
void
canvas_set_width(struct Canvas *s, int width)
{
	s->width = width;
}

void
canvas_set_height(struct Canvas *s, int height)
{
	s->height = height;
}

int
canvas_get_width(struct Canvas *s)
{
	return s->width;
}

int
canvas_get_height(struct Canvas *s)
{
	return s->height;
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

CanvasColor *
canvas_get_colors_bg(struct Canvas *s)
{
	return s->colors_bg;
}

CanvasColor *
canvas_get_colors_fg(struct Canvas *s)
{
	return s->colors_fg;
}

void
destroy_canvas(struct Canvas *s)
{
	free(canvas_get_canvas(s));
	free(s);
}

void
show_canvas(struct Canvas *c, WINDOW *w)
{
	int height = canvas_get_height(c);
	int width = canvas_get_width(c);
	char *canvas = canvas_get_canvas(c);
	CanvasColor *colors_fg = canvas_get_colors_fg(c);

	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			setcolor(colors_fg[y * width + x], 0, w);
			/* offset printing because of borders */
			mvwprintw(w,y+1, x+1, "%c", canvas[y * width + x] );
			unsetcolor(colors_fg[y * width + x], 0, w);
		}
	}
}

void
canvas_clear(struct Canvas *s)
{
	char *c = s->canvas;
	int n=s->height * s->width - 1;
	while (n) c[n--] = BLANK;

	CanvasColor *colors_fg = s->colors_fg;
	n=s->height * s->width - 1;
	while (n) colors_fg[n--] = COLOR_BLACK;
}

struct Canvas *
new_canvas(int height, int width)
{
	struct Canvas *s = malloc(sizeof(struct Canvas));
	canvas_set_height(s, height);
	canvas_set_width(s, width);
	s->canvas = malloc(sizeof(char) * (height) * (width));
	s->colors_fg = malloc(sizeof(int) * (height) * (width));
	s->colors_bg = malloc(sizeof(int) * (height) * (width));

	// clear canvas
	canvas_clear(s);

	return s;
}

/* Color functions taken from
 * https://www.linuxjournal.com/content/about-ncurses-colors-0
 */
CanvasColor
curs_color(int fg)
{
	CanvasColor color;

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
