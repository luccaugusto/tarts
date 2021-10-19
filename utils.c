#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "constants.h"

char *
int2str(int n)
{
	int length = snprintf(NULL, 0,"%d",n);
	char *str = malloc(sizeof(char) * (length + 1));

	sprintf(str, "%d", n);
	return str;
}

char *
double2str(double n)
{
	int length = snprintf(NULL, 0,"%.2f",n);
	char *str = malloc(sizeof(char) * (length + 1));

	sprintf(str, "%.2f", n);
	return str;
}

double
str2double(char *str)
{
	double d;
	sscanf(str, "%lf", &d);

	return d;
}

struct ValueList *
parse_values(char *value_string)
{
	/* there is one less , for the number of values on a csv list */
	char *aux;
	int old_i = 0;
	int count_values = 1;
	int parsed_values = 0;
	int string_len = strlen(value_string);
	struct ValueList *vlist = malloc(sizeof(struct ValueList));

	for (int i=0; i<string_len; ++i) {
		if (value_string[i] == ',')
			count_values++;
	}

	vlist->values = malloc(sizeof(double) * count_values);
	vlist->count_values = count_values;

	aux = malloc(string_len);
	for (int i=0; i<string_len; ++i) {
		if (value_string[i] == ',') {
			/* copy to aux value until ,*/
			strncpy(aux, &value_string[old_i], i-old_i);
			vlist->values[parsed_values++] = str2double(aux);
			old_i = i+1; /* skip the , */
		}

		if (i + 1 == string_len) {
			strncpy(aux, &value_string[old_i], string_len-old_i);
			vlist->values[parsed_values++] = str2double(aux);
		}
	}
	free(aux);
	return vlist;
}
