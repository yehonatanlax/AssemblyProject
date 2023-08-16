#ifndef _EXTERN_H_
#define _EXTERN_H_


typedef struct {
    char* name;
    char** instances;
    int num_of_instances;
} Extern;

void handle_extern(char* directive, char* line, int line_number);

void add_extern(char* name);

void add_instance_to_extern(char** address);

int is_extern_exist(char* name);

int is_valid_extern( char* extern_word, char* line, int line_number);

#endif