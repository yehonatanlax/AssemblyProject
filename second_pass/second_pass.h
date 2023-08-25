#ifndef _SECOND_PASS_H_
#define _SECOND_PASS_H_

#include <stdio.h>


void second_pass(char* file_name);

FILE* create_file(char* kind_file, char* file_name);

void add_to_file(FILE* file, char* name_label, int address);

void remove_files(char* name);

void create_ob_file(char* file_name);

#endif