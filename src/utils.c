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
			aux[i-old_i] = '\0';
			vlist->values[parsed_values++] = str2double(aux);
			old_i = i+1; /* skip the , */
		}

		if (i + 1 == string_len) {
			strncpy(aux, &value_string[old_i], string_len-old_i);
			aux[string_len-old_i] = '\0';
			vlist->values[parsed_values++] = str2double(aux);
		}
	}
	free(aux);
	return vlist;
}

struct LabelList *
parse_labels(char *label_string)
{
	/* there is one less , for the number of labels on a csv list */
	char *aux;
	int old_i = 0;
	int count_labels = 1;
	int parsed_labels = 0;
	int string_len = strlen(label_string);
	struct LabelList *llist = malloc(sizeof(struct LabelList));

	for (int i=0; i<string_len; ++i) {
		if (label_string[i] == ',')
			count_labels++;
	}

	llist->labels = malloc(sizeof(char *) * count_labels);
	llist->count_labels = count_labels;

	for (int i=0; i<string_len; ++i) {
		if (label_string[i] == ',') {
			/* copy to aux value until ,*/
			aux = malloc(sizeof(char) * i-old_i);
			strncpy(aux, &label_string[old_i], i-old_i);
			llist->labels[parsed_labels++] = aux;
			old_i = i+1; /* skip the , */
		}

		if (i + 1 == string_len) {
			aux = malloc(sizeof(char) * string_len-old_i);
			strncpy(aux, &label_string[old_i], string_len-old_i);
			llist->labels[parsed_labels++] = aux;
		}
	}
	return llist;
}
