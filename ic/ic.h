#ifndef _IC_H_
#define _IC_H_

typedef struct {
    char** ic_data;
    int ic_counter;
} IC;

int push_ic(char** operands, char* instruction, int operands_num, int* op_types, int line_number, char* op_to_bl);

int push_single_ic(char* ic);

void print_all_IC();

void free_memory_ic();

void initialize_ic();

#endif