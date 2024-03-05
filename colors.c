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

short
get_color_by_name(char color_name) {
	if ('A' <= color_name && color_name <= 'Z') color_name += 32;

	short color = color_list[0];

	//set color to color -1 as get_color will increment it
	switch (color_name) {
		case 'b':
			color = color_list[0];
			break;
		case 'g':
			color = color_list[1];
			break;
		case 'c':
			color = color_list[2];
			break;
		case 'r':
			color = color_list[3];
			break;
		case 'm':
			color = color_list[4];
			break;
		case 'y':
			color = color_list[5];
			break;
		case 'w':
			color = color_list[6];
			break;
		default: /* do nothing */
			break;
	}

	return color;
}
