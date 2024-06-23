#ifndef PIE_CHART_H
#define PIE_CHART_H

#define PIE_OFFSET            1.5
#define MAX_PORTIONS          100
#define CIRCLE_DEGREE_COUNT   360
#define MAX_RADIUS            12 /* radius > 14 breaks colors for some reason */

typedef enum {
	PIEBLOCK_FULL   = 'O',
	PIEBLOCK_TOP    = '"',
	PIEBLOCK_BOTTOM = '_',
} PIEBLOCKS;

typedef struct PieChart Pie;
typedef struct Portion Portion;

Pie *new_pie (double center_y, double center_x);
Portion *new_portion(int percentage, double value, char *name, Color color);
PlotStatus print_pie(void *p, struct Dimentions *dimentions, char *canvas_screen, Color *canvas_colors);
double get_center_x(Pie *p);
double get_center_y(Pie *p);
void set_center_x(Pie *p, double center_x);
void set_center_y(Pie *p, double center_y);
double get_radius(Pie *p);
void pie_push_portion(Pie *pie, Portion *portion);
void pie_pop_portion(Pie *p);
Portion *get_portion_by_index(Pie *p, int index);
int get_portion_count(Pie *p);
int portion_get_value(Portion *p);
int pie_get_stats(Pie *p, double *min, double *max, double *avg, double *total, char *min_label, char *max_label);

#endif
