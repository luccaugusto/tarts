#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FRAMED              0
#define PADDING             4
#define BLANK               ' '
#define MAX_NAME_LENGTH     50
#define SCALE_INCREMENT     0.05
#define PI                  3.14159265
#define RADIANS_TO_DEG(x)   (x * 180.0/PI)
#define MAX_CHARTS          10
#define FOOTER_HEIGHT       3
#define VERSION             "Tarts 1.0"
#define MANTAINER_EMAIL     "lucca@luccaaugusto.xyz"

#define CODE_BLACK       "\e[0;30m"
#define CODE_RED         "\e[0;31m"
#define CODE_GREEN       "\e[0;32m"
#define CODE_YELLOW      "\e[0;33m"
#define CODE_BLUE        "\e[0;34m"
#define CODE_MAGENTA     "\e[0;35m"
#define CODE_CYAN        "\e[0;36m"
#define CODE_WHITE       "\e[0;37m"
#define CODE_COLOR_RESET "\e[0m"

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
} PlotStatus;

typedef enum {
	BAR_CHART,
	PIE_CHART,
	LINE_CHART,
	COUNT_CHARTS,
} ChartType;

typedef PlotStatus (*PlotFunction)(void *, struct Dimentions *, char *, Color *);

#endif
