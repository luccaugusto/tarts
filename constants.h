#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FRAMED           1
#define HEIGHT           30
#define WIDTH            124
#define PADDING          2
#define BLANK            ' '
#define MAX_NAME_LENGTH  50

enum Frame {
	FRAME_CORNER   = '+',
	FRAME_BORDER_H = '-',
	FRAME_BORDER_V = '|',
};

enum Pixel {
	BACK = 0,
	FORE = 1,
};

#endif
