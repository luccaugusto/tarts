#include <ncurses.h>
#include "colors.h"

short color_list[] = {
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
};

int count_color = 7;
int color_used = 0;

short
get_color()
{
	/* just cicle on list to allow more than 7 portions */
	return color_list[color_used = (color_used + 1) % count_color];
}

