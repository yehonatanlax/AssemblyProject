#ifndef _LABELS_H_
#define _LABELS_H_

typedef struct {
    char* name;
    int address;
    int params_counter;
    int type;
} Label;

typedef struct {
    int line_number;
    int instances_num;
    int* instances;
    char* name;
    int num_line;
} Blacked_Label;

void add_label(char* label, int num_of_lines, int address, int line_number, int type);

void add_to_labels_black_list(char* label, int line_number, int address);

void remove_label_from_black_list(char* label);

int is_label_exist(char* label);

Label* getLabel(char* label);

int is_black_list_empty();

int is_valid_entry( char* entry_word, char* line, int line_number);

void handle_entry_label(char* directive, char* line, int line_number);

Blacked_Label* is_black_label_exist(char* label);

void print_labels();

void get_black_list();

void free_memory_label();

void initialize_labels();

#endif