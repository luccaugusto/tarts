#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "bar_chart.h"

/* TYPES */
struct BarChart {
	int number;
	char name[MAX_NAME_LENGTH];
};

/* FUNCTION DEFINITIONS */

void
print_bar(Bar *series, char *chart, int x_offset)
{
	int width = WIDTH - 2; //offset things
	int height = HEIGHT - 1; //offset things
	int y_offset = 1;
	int name_len = strlen(get_name(series));

	x_offset = (x_offset < 1) ? 1 : x_offset;
	x_offset += name_len / 2;

	int i = y_offset;
	for (; i<height; ++i) {
		chart[i * WIDTH + x_offset] = (i > height - get_number(series)) ?
			BARBLOCK : BLANK;
	}

	strncpy(&chart[i * WIDTH + x_offset - name_len/2], get_name(series), name_len);
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
	int len = (strlen(name) > MAX_NAME_LENGTH) ?
		MAX_NAME_LENGTH : strlen(name);
	strncpy(series->name, name, len);
}

struct BarChart *
new_bar (int number, char *name)
{
	int len = (strlen(name) > MAX_NAME_LENGTH) ?
		MAX_NAME_LENGTH : strlen(name);

	Bar *b = malloc(sizeof(Bar));
	b->number = number;
	strncpy(b->name, name, len);
	return b;
}
