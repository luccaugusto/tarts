#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FRAMED              0
#define PADDING             4
#define BLANK               ' '
#define MAX_NAME_LENGTH     50
#define SCALE_STEP          0.1
#define ZOOM_IN  		    1
#define ZOOM_OUT 		    0
#define PI                  3.14159265
#define RADIANS_TO_DEG(x)   (x * 180.0/PI)
#define MAX_CHARTS          10
#define FOOTER_HEIGHT       3
#define VERSION             "Tarts 1.0"
#define MANTAINER_EMAIL     "lucca@luccaaugusto.xyz"

#define CODE_BLACK       "\033[0;30m"
#define CODE_RED         "\033[0;31m"
#define CODE_GREEN       "\033[0;32m"
#define CODE_YELLOW      "\033[0;33m"
#define CODE_BLUE        "\033[0;34m"
#define CODE_MAGENTA     "\033[0;35m"
#define CODE_CYAN        "\033[0;36m"
#define CODE_WHITE       "\033[0;37m"
#define CODE_COLOR_RESET "\033[0m"

typedef short Color;

#define COLOR_BLANK (Color)0

typedef enum {
	FRAME_CORNER   = '+',
	FRAME_BORDER_H = '-',
	FRAME_BORDER_V = '|',
} Frame;

typedef enum {
	PLOT_OK = 0,
	ERR_CIRCLE_TOO_BIG,
	ERR_BAR_TOO_BIG,
	ERR_LINE_OUT,
	ERR_ARGS_COUNT_DONT_MATCH,
	ERR_TOO_MANY_CHARTS,
} PlotStatus;

typedef enum {
	BAR_CHART,
	PIE_CHART,
	LINE_CHART,
	COUNT_CHART_TYPES,
} ChartType;

typedef PlotStatus (*PlotFunction)(void *, struct Dimentions *, char *, Color *);

#endif
