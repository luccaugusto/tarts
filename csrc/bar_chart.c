#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "bar_chart.h"

/* TYPES */
struct BarChart {
	int number;
	char name[MAX_NAME_LENGHT];
};

/* FUNCTION DEFINITIONS */

void
print_bar(Bar *series, char *chart, int x_offset)
{
	int width = WIDTH;
	int height = HEIGHT;
	int y_offset = 0;

	if (FRAMED) {
		width -= 2;
		height -= 1;
		y_offset = 1;
		x_offset = (x_offset < 1) ? x_offset + 1 : x_offset;
	}

	for (int i=y_offset; i<height; ++i) {
		chart[i * WIDTH + x_offset] = (i > height - get_number(series)) ?
			BARBLOCK : BLANK;
	}
}

int
get_number (Bar *series)
{
	return series->number;
}

char *
get_name (Bar *series)
{
	return series->name;
}

void
set_number (Bar *series, int number)
{
	series->number = number;
}

void
set_name (Bar *series, char *name)
{
	int len = (strlen(name) > MAX_NAME_LENGHT) ?
		MAX_NAME_LENGHT : strlen(name);
	strncpy(series->name, name, len);
}

struct BarChart *
new_bar (int number, char *name)
{
	int len = (strlen(name) > MAX_NAME_LENGHT) ?
		MAX_NAME_LENGHT : strlen(name);

	Bar *b = malloc(sizeof(Bar));
	b->number = number;
	strncpy(b->name, name, len);
	return b;
}
