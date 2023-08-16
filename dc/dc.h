#ifndef _DC_H_
#define _DC_H_

typedef struct {
    char** dc_data;
    int dc_counter;
} DC;

void add_string(char* str, char* label, int line_number);

void add_data(char* str, char* label, int line_number);

char** push_str(char* str);

#endif