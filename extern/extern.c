#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../functions/functions.h"
#include "../labels/labels.h"
#include "extern.h"

Extern *externs = NULL; /*Array pointers of externs*/
int num_externs = 0;

void handle_extern(char* directive, char* line, int line_number){
    char* name_of_extern = NULL;
    move_line_ptr_to_next_word(directive, line);
    name_of_extern = getFirstWord(line);
    if (is_valid_extern(name_of_extern, line, line_number)) {
        add_extern(name_of_extern);
    }

    free(name_of_extern);
}

void add_extern(char* name) {
    externs = realloc(externs, (num_externs + 1) * sizeof(Extern));
    externs[num_externs].name = malloc(strlen(name) + 1);
    strcpy( externs[num_externs].name, name);
    externs[num_externs].num_of_instances = 0;
    printf("extern added: %s\n",  externs[num_externs].name);
    num_externs++;
}

int is_extern_exist(char* name){
    int i;
    if (externs != NULL){
        for (i = 0; i < num_externs; i++){
            if (strcmp(name,externs[i].name) == 0){
                return 1;
            }
        }
    }

    return 0;
}

int is_valid_extern( char* extern_word, char* line, int line_number) { 
    char* check_more_param = NULL;
    move_line_ptr_to_next_word(extern_word, line);
    check_more_param = getFirstWord(line);
    if (!(check_more_param[0] == '\0')) {
        handleError("too many params for extern", line_number);
    }
    else if (isDirective(extern_word)){
        handleError("invalid name. name of extern is like a directve", line_number);
    }
    else if (isInstruction(extern_word)){
        handleError("invalid name. name of extern is like a instruction", line_number);
    }
    else if (!can_word_be_valid_label(extern_word, line_number)) {
        handleError("invalid name. invalid extern name", line_number);
    }
    else if (is_extern_exist(extern_word)){
        handleError("the name of exterm already exist", line_number);
    }
    else {
        free(check_more_param);
        return 1;
    }

    free(check_more_param);
    return 0;
}