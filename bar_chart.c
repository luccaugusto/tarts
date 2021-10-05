#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "bar_chart.h"
#include "utils.h"

/* TYPES */
struct BarChart {
	float number;
	char name[MAX_NAME_LENGTH];
	Color color;
	int x_offset;
};

/* FUNCTION DEFINITIONS */

void
print_bar_chart(Bar *series, int width, int height, char *canvas_screen, Color *canvas_colors, float scale)
{
	int y_offset = 1;
	int plotted_number = 0;
	height -= 1;
	int name_len = strlen(bar_get_name(series));
	float diff;
	float diff_margin = 1.0;
	float scaled_height = height - (bar_get_number(series) * scale);
	scaled_height = (scaled_height >= height) ? height - 1 : scaled_height;

	int x_offset = (series->x_offset > width) ? series->x_offset % width : ((series->x_offset < 1) ? 1 : series->x_offset);
	x_offset += name_len / 2;

	int i = y_offset;
	for (; i<height; ++i) {
		diff = (float) i - scaled_height;

		/* print number on top of bar */
		if (-diff_margin < diff && diff < diff_margin && !plotted_number) {
			char *str = float2str(bar_get_number(series));
			int number_len = strlen(str);

			int j = (i * width + x_offset - 1 - number_len/2);
			int k = j + number_len;

			strncpy(&canvas_screen[j], str, number_len);

			/* color number */
			for (; j<k; ++j)
				canvas_colors[j] = series->color;

			plotted_number = 1;
			free(str);
		} else if (i > scaled_height) {
			canvas_screen[i * width + x_offset] = BARBLOCK;
			canvas_colors[i * width + x_offset] = series->color;
		} else {
			canvas_screen[i * width + x_offset] = BLANK;
		}
	}

	/* color name */
	int j = (i * width + x_offset - name_len/2);
	int k = j + name_len;
	strncpy(&canvas_screen[j], bar_get_name(series), name_len);
	for (; j < k; ++j)
		canvas_colors[j] = series->color;
}

float
bar_get_number (Bar *series)
{
	return series->number;
}

char *
bar_get_name (Bar *series)
{
	return series->name;
}

void
bar_set_number (Bar *series, float number)
{
	series->number = number;
}

void
bar_set_name (Bar *series, char *name)
{
	int len = (strlen(name) > MAX_NAME_LENGTH) ?
		MAX_NAME_LENGTH : strlen(name);
	strncpy(series->name, name, len);
}

Color
bar_get_color (Bar *series)
{
	return series->color;
}

void
bar_set_color (Bar *series, Color color)
{
	series->color = color;
}

struct BarChart *
new_bar (float number, char *name)
{
	int len = (strlen(name) > MAX_NAME_LENGTH) ?
		MAX_NAME_LENGTH : strlen(name);

	Bar *b = malloc(sizeof(Bar));
	b->number = number;
	strncpy(b->name, name, len);
	return b;
}
