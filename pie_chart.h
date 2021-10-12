#ifndef PIE_CHART_H
#define PIE_CHART_H

#define PIE_OFFSET            1.5
#define MAX_PORTIONS          100
#define CIRCLE_DEGREE_COUNT   360

typedef enum {
	PIEBLOCK_FULL   = 'O',
	PIEBLOCK_TOP    = '"',
	PIEBLOCK_BOTTOM = '_',
} PIEBLOCKS;

typedef struct PieChart Pie;
typedef struct Portion Portion;

Pie *new_pie (double center_x, double center_y, double radius, double total);
Portion *new_portion(int percentage, char *name, Color color);
int print_pie(Pie *pie, struct Dimentions *dimentions, char *canvas_screen, Color *canvas_colors);
double get_center_x(Pie *p);
double get_center_y(Pie *p);
void set_center_x(Pie *p, double center_x);
void set_center_y(Pie *p, double center_y);
double get_radius(Pie *p);
double get_total(Pie *p);
void pie_push_portion(Pie *pie, Portion *portion);
void pie_pop_portion(Pie *pie);
void pie_show_stack(Pie *pie);

#endif
