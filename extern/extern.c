#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../functions/functions.h"
#include "../labels/labels.h"
#include "extern.h"

Extern *externs = NULL; /* Array pointers of externs */
int num_externs = 0;

/* the functions handles an extern line */
void handle_extern(char* directive, char* line, int line_number){
    char* name_of_extern = get_first_word(line);
    /* if the extern label is valid - add to externs table */
    if (is_valid_extern(name_of_extern, line, line_number)) {
        add_extern(name_of_extern);
    }

    free(name_of_extern);
}

/* add extern to extern table */
void add_extern(char* name) {
    /* allocate new place for the new Extern obj */
    externs = realloc(externs, (num_externs + 1) * sizeof(Extern));
    externs[num_externs].name = malloc(strlen(name) + 1);
    strcpy( externs[num_externs].name, name);
    externs[num_externs].num_of_instances = 0;
    num_externs++;
}

/* checks if there is already an extern with this name */
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

/* checks if the given name could be a valid extern obj */
int is_valid_extern( char* extern_word, char* line, int line_number) { 
    char* check_more_param = NULL;
    move_line_ptr_to_next_word(extern_word, line);
    check_more_param = get_first_word(line); /* checks if there are other words after decleration */
    /* if there are other words after decleration */
    if (!(check_more_param[0] == '\0')) {
        handleError("too many params for extern", line_number);
    }
    /* checks if the name of thee extern can be avalid label name */
    else if (!can_word_be_valid_label(extern_word, line_number)) {
        handleError("invalid extern name", line_number);
    }
    /* checks if this name already exists in externs table */
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

/* free externs table */
void free_memory_extern(){
    free(externs);
}

/* initialize externs table */
void initialize_extern() {
    externs = NULL; /*Array pointers of externs*/
    num_externs = 0;
}