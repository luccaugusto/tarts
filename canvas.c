#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "./canvas.h"

struct Canvas{
	int height;
	int width;
	char *canvas;
};

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

void
destroy_canvas(struct Canvas *s)
{
	free(canvas_get_canvas(s));
	free(s);
}

void
show_canvas(struct Canvas *s, WINDOW *w)
{
	int height = canvas_get_height(s);
	int width = canvas_get_width(s);
	char *canvas = canvas_get_canvas(s);

	for (int y=1; y<height; ++y) {
		for (int x=1; x<width; ++x) {
			mvwprintw(w,y, x, "%c", canvas[y * width + x] );
		}
	}
}

void
canvas_clear(struct Canvas *s)
{
	char *p = s->canvas;
	int n=s->height * s->width - 1;
	while (n) p[n--] = BLANK;
}

struct Canvas *
new_canvas(int height, int width)
{
	struct Canvas *s = malloc(sizeof(struct Canvas));
	canvas_set_height(s, height);
	canvas_set_width(s, width);
	s->canvas = malloc(sizeof(char) * (height) * (width));

	// clear canvas
	canvas_clear(s);

	return s;
}
