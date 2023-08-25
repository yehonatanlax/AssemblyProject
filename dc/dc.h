#ifndef _DC_H_
#define _DC_H_

typedef struct {
    char* label;
    char** data;
    int num;
} DC_obj;

typedef struct {
    DC_obj* dc_data;
    int dc_counter;
} DC;

void add_string(char* str, char* label, int line_number);

void add_data(char* str, char* label, int line_number);

int push_data(char** data, int num, int line_number, int flag, char* label);

void print_all_DC(); // TODO: remove

void free_memory_dc();

void initialize_dc();

#endif