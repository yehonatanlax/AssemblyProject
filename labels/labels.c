#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "labels.h"
#include "../functions/functions.h"
#include "../extern/extern.h"
#include "../ic/ic.h"

/* Labels Table */
Label* labels = NULL;
int num_labels = 0;

/* Black List Table - for labels in use in first pass */
Blacked_Label* labels_black_list = NULL;
int black_list_num = 0;

/* add label to labels table */
void add_label(char* label, int num_of_lines, int address, int line_number, int type) {
    Label* label_obj = getLabel(label);
    if (label_obj == NULL) {
        /* Allocating new place for the new label */
        labels = realloc(labels, (num_labels + 1) * sizeof(Label));
        labels[num_labels].address = address;
        labels[num_labels].params_counter = num_of_lines;
        labels[num_labels].type = type;
        labels[num_labels].name = malloc(strlen(label) + 1);
        strcpy(labels[num_labels].name, label);
        num_labels++;
    } 
    else if (label_obj->type == 0 && label_obj->address == -1) { /* handle entry label */
        label_obj->address = address;
    }
    else if (label_obj->type == 2 && label_obj->address == -1) { /* handle dc label */
        label_obj->address = address;
        label_obj->type = type; 
    }
    else {
        handleError("The name of this label already exists in labels table", line_number);
    }
}

/* if label doesn't exist yet, add it to black list which will be checked at the end */
void add_to_labels_black_list(char* label, int line_number, int address) {
    Blacked_Label* label_obj = is_black_label_exist(label);
    if (labels_black_list == NULL) {
        labels_black_list = malloc(sizeof(Blacked_Label));
    }
    if (label_obj == NULL) {
        labels_black_list = realloc(labels_black_list, (black_list_num + 1) * sizeof(Blacked_Label));
        labels_black_list[black_list_num].line_number = line_number;
        labels_black_list[black_list_num].name = malloc(strlen(label) + 1);
        labels_black_list[black_list_num].instances_num = 1;
        labels_black_list[black_list_num].instances = malloc(sizeof(int*));
        labels_black_list[black_list_num].instances[0] = address;
        labels_black_list[black_list_num].line_number = line_number;
        strcpy(labels_black_list[black_list_num].name, label);
        black_list_num++;
    }
    else {
        label_obj->instances = realloc(label_obj->instances, sizeof(int*)*(label_obj->instances_num + 1));
        label_obj->instances[label_obj->instances_num] = address;
        label_obj->instances_num++;
    }
}

/* remove label from black list */
void remove_label_from_black_list(char* label) {
    int i;
    int size = strlen(label);
    char* without_colon = (char*)malloc(size*sizeof(char));
    memset(without_colon, '\0', size);
    strncpy(without_colon, label, size-1);
    if (!is_black_list_empty()) {
        for (i = 0; i < black_list_num; i++) {
            if (!strcmp(without_colon, labels_black_list[i].name)) {
                if (black_list_num == 1) {
                    free(labels_black_list);
                    labels_black_list = NULL;
                }
                else {
                    strcpy(labels_black_list[i].name, labels_black_list[black_list_num - 1].name);
                    labels_black_list[i].line_number = labels_black_list[black_list_num - 1].line_number;
                    free(labels_black_list[black_list_num - 1].name);
                }
                black_list_num--;
            }
        }
    }
}

/* handle label of entry type */
void handle_entry_label(char* directive, char* line, int line_number){
    Label* label = NULL;
    char* name_of_entry = get_first_word(line);
    if (is_valid_entry(name_of_entry, line, line_number)) {
        label = getLabel(name_of_entry);
        if (label != NULL) {
            if (label->type == 0) {
                handleError("entry already declared", line_number);
            }
            else if (label->type == 1) {
                label->type = 0;
            }
        }
        else {   
            /* add label with type = 0 (to sign that it's an entry) */
            add_label(name_of_entry, 1, -1, line_number, 0);
        }
    }

    free(name_of_entry);
}

/* check if word can be a valid entry label */
int is_valid_entry( char* entry_word, char* line, int line_number) { 
    char* check_more_param = NULL;
    move_line_ptr_to_next_word(entry_word, line);
    check_more_param = get_first_word(line);
    if (!(check_more_param[0] == '\0')) {
        handleError("too many params for entry", line_number);
    }
    else if (is_directive(entry_word)){
        handleError("invalid name. name of entry is like a directve", line_number);
    }
    else if (is_instruction(entry_word) != -1){
        handleError("invalid name. name of entry is like a instruction", line_number);
    }
    else if (!can_word_be_valid_label(entry_word, line_number)) {
        handleError("invalid name. invalid entry name", line_number);
    }
    else {
        free(check_more_param);
        return 1;
    }

    free(check_more_param);
    return 0;
}

/* checks if black list is empty */
int is_black_list_empty() {
    return labels_black_list == NULL;
}

/* checks if label exists in labels table */
int is_label_exist(char* label) {
    int size;
    char* new_str;
    Label* label_obj = NULL;
    size = strlen(label);
    new_str = (char*)calloc(size,sizeof(char));
    if (label[size-1] == ':'){
        strncpy(new_str,label,size-1);
        strcpy(label,new_str);
    }

    free(new_str);
    label_obj = getLabel(label);
    return label_obj != NULL && label_obj->address != -1; 
}

/* checks if label exists in labels table */
Blacked_Label* is_black_label_exist(char* label) {
    int i;
    /* if labels is not empty */
    if (labels_black_list != NULL) {
        /* go over all labels in labels data */
        for (i = 0; i < black_list_num; i++) {
            if (strcmp(label, labels_black_list[i].name) == 0) {
                return &labels_black_list[i];
            }
        }
    }

    return NULL;
}

/* returns label data */
Label* getLabel(char* label) {
    int i;
    /* if labels is empty */
    if (labels != NULL) {
        for (i = 0; i < num_labels; i++) {
            if (strcmp(label, labels[i].name) == 0) {
                return &labels[i];
            }
        }
    }

    return NULL;
}

/* free Labels and Black List tables */
void free_memory_label() {
    free(labels);
    free(labels_black_list);
}

/* initialize Labels and Black List tables */
void initialize_labels() {
    labels = NULL;
    num_labels = 0;
    black_list_num = 0;
    labels_black_list = NULL;
}