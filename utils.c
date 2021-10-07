#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
