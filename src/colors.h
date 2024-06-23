#ifndef COLORS_H
#define COLORS_H

#define BLANK_COLOR -1

extern short color_list[];
extern int count_color;
extern int color_used;
extern short chart_colors[];

short get_color();
short get_color_by_name(char color);

#endif
