#ifndef _MACRO_H_
#define _MACRO_H_


typedef struct {
    char* name;
    char* content;
} Macro;

char* checkIfItsMacro(char * line);

char* preprocessor(const char* filename, int line_number);

char* replace_macro_content(const char* filepath);

void add_macro(char* name, char* content, int line_number);

void free_memory_macro();


#endif