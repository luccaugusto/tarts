/* HEADERS */
#include <ncurses.h>
#include <panel.h>
#include <string.h>
#include <stdlib.h>

#include "./utils.h"
#include "./constants.h"
#include "./canvas.h"
#include "./tart.h"

/* GLOBAL VARIABLES */
WINDOW *prompt_win;
PANEL *prompt_panel;

char buffer[MAX_NAME_LENGTH];

void
print_align_center(WINDOW *win, int start_y, int start_x, int width, char *string, chtype color)
{
	int length, x, y;

	win = (win == NULL) ? stdscr : win;

	getyx(win, y, x);

	x = (start_x == 0) ? x : start_x;
	y = (start_y == 0) ? y : start_y;

	length = strlen(string);
	x = start_x + (width - length) / 2;
	wattrset(win, color);
	mvwprintw(win, y, x, "%s", string);
	refresh();
}

void
draw_headers(WINDOW *window, int height, int width, char *label, chtype color)
{
	wattrset(window, color);
	mvwaddch(window, 2, 0, ACS_LTEE);
	mvwhline(window, 2, 1, ACS_HLINE, width - 2);
	mvwaddch(window, 2, width - 1, ACS_RTEE);
	print_align_center(window, 1, 0, width, label, color);
	attroff(color);
}

char *
prompt_user(Canvas *c, char *question, char *label, int align_center)
{
	char *answer;
	int prompt_win_h = canvas_get_height(c) / 3;
	int prompt_win_w = canvas_get_width(c) / 4;
	WINDOW *p_win = panel_window(prompt_panel);;

	wattrset(p_win, COLOR_PAIR(colornum(7,0)));
	box(p_win, 0, 0);
	draw_headers(p_win, prompt_win_h, prompt_win_w, label, COLOR_PAIR(colornum(7,0)));

	if (align_center)
		print_align_center(p_win, HEADER_HEIGHT, 1, prompt_win_w, question, COLOR_PAIR(colornum(7,0)));
	else
		mvwprintw(p_win, HEADER_HEIGHT, 1, question);

	show_panel(prompt_panel);

	echo();
	wgetstr(p_win, buffer);
	noecho();

	werase(p_win);
	hide_panel(prompt_panel);

	answer = malloc(sizeof(char) * strlen(buffer));
	strcpy(answer, buffer);

	return answer;
}

void
alert(Canvas *c, char *alert, char *label, int align_center)
{
	int prompt_win_h;
	int prompt_win_w;
	WINDOW *p_win = panel_window(prompt_panel);
	getmaxyx(p_win, prompt_win_h, prompt_win_w);

	wattrset(p_win, COLOR_PAIR(colornum(7,0)));
	box(p_win, 0, 0);
	draw_headers(p_win, prompt_win_h, prompt_win_w, label, COLOR_PAIR(colornum(7,0)));

	if (align_center)
		print_align_center(p_win, HEADER_HEIGHT, 1, prompt_win_w, alert, COLOR_PAIR(colornum(7,0)));
	else
		mvwprintw(p_win, HEADER_HEIGHT, 2, alert);

	mvwprintw(p_win, prompt_win_h - 1, prompt_win_w/2, "[OK]");

	show_panel(prompt_panel);

	wgetch(p_win);

	werase(p_win);
	hide_panel(prompt_panel);
}

// void
// prompt_add_note(short tag, short priority)
// {
// 	int intput;
// 	int n_pri = DEFAULT_PRIORITY;
// 	char *input;
// 	char *label;
// 	char *label2;
// 	char *label3;
// 	char *n_tag = DEFAULT_TAG;
// 	Note n_aux;
//
// 	if (tag && priority) {
// 		label = "Adding [note], priority and tag";
// 		label2 = "Adding note, [priority] and tag";
// 		label3 = "Adding note, priority and [tag]";
// 	} else if (tag) {
// 		label = "Adding [note] and tag";
// 		label2 = "";
// 		label3 = "Adding note and [tag]";
// 	} else if (priority) {
// 		label = "Adding [note] and priority";
// 		label2 = "Adding note and [priority]";
// 		label3 = "";
// 	} else {
// 		label = "Adding note";
// 		label2 = "";
// 		label3 = "";
// 	}
//
// 	/* show cursor */
// 	curs_set(1);
//
// 	input = prompt_user("Note text [blank to cancel]: ", label, ALIGN_LEFT);
//
// 	if (is_blank(input))
// 		return;
//
// 	while (in_str('|', input) || strlen(input) > main_win_w - 2)
// 		input = prompt_user("Note too long or '|' detected, try again:\n|", label, ALIGN_LEFT);
//
// 	n_aux = new_note(input);
//
// 	if (priority) {
// 		intput = str2int(prompt_user("Note priority [0-9]: ", label2, ALIGN_LEFT));
//
// 		while (intput < 0 || 9 < intput)
// 			intput = str2int(prompt_user("Type a valid number please [0-9]: ", label2, ALIGN_LEFT));
//
// 		n_pri = intput;
// 	}
//
// 	if (tag) {
// 		input = prompt_user("On which tag? [blank for default]: ", label3, ALIGN_LEFT);
// 		if (!is_blank(input)) {
// 			n_tag = input;
// 		}
// 	}
//
// 	note_set_priority(n_pri, n_aux);
// 	tag_add_note(n_aux, n_tag);
//
// 	/* hide cursor again */
// 	curs_set(0);
//
// 	reload_main_win();
// }
