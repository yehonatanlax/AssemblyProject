#ifndef _IC_H_
#define _IC_H_

typedef struct {
    char** ic_data;
    int ic_counter;
} IC;

char** push_ic(char** operands, char* instruction, int operands_num, int* op_types, int line_number);

char** push_single_ic(char* ic);

void print_all_IC();

#endif