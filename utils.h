/* TYPES */
struct Dimentions {
	int width;
	int height;
	double scale;
};

struct ValueList {
	double *values;
	int count_values;
};

struct LabelList {
	char **labels;
	int count_labels;
};

/* FUNCTION PROTOTYPES */
char *int2str(int n);
char *double2str(double n);
double str2double(char *str);
struct ValueList *parse_values(char *value_string);
struct LabelList *parse_labels(char *label_string);
