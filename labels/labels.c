#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "labels.h"
#include "../functions/functions.h"
#include "../extern/extern.h"
#include "../ic/ic.h"

Label* labels = NULL;
int num_labels = 0;
int black_list_num = 0;
Blacked_Label* labels_black_list = NULL;

void print_labels(){
    int i;
    for (i = 0; i < num_labels; i++){
        printf("label name: %s - type: %d\n",labels[i].name, labels[i].type);
    }
}

/* add label to labels table */
void add_label(char* label, int num_of_lines, char** address, int line_number, int type) {
    /*int size;*/
    if (!is_label_exist(label)) {
        /* Allocating new place for the new label */
        labels = realloc(labels, (num_labels + 1) * sizeof(Label));
        labels[num_labels].address = address;
        labels[num_labels].params_counter = num_of_lines;
        labels[num_labels].type = type;
        labels[num_labels].name = malloc(strlen(label) + 1);
        /*size = strlen(label);*/
        strcpy((labels)[num_labels].name, label);
        /*(labels)[num_labels].name[size-1] = '\0'; copy label without colon */
        num_labels++;
    } else {
        handleError("The name of this label already exists in labels table", line_number);
    }
}

/* if label doesn't exist yet, add it to black list which will be checked at the end */
void add_to_labels_black_list(char* label, int line_number, char** address) {
    int i;
    Blacked_Label* label_obj = is_black_label_exist(label);
    if (labels_black_list == NULL) {
        labels_black_list = malloc(sizeof(Blacked_Label));
    }
    if (label_obj == NULL) {
        labels_black_list = realloc(labels_black_list, (black_list_num + 1) * sizeof(Blacked_Label));
        labels_black_list[black_list_num].line_number = line_number;
        labels_black_list[black_list_num].name = malloc(strlen(label) + 1);
        labels_black_list[black_list_num].instances_num = 1;
        labels_black_list[black_list_num].instances = malloc(sizeof(char**));
        labels_black_list[black_list_num].instances[0] = realloc(labels_black_list[black_list_num].instances, sizeof(char*));
        labels_black_list[black_list_num].instances[labels_black_list[black_list_num].instances_num - 1] = *address;
        strcpy(labels_black_list[black_list_num].name, label);
        black_list_num++;
    }
    else {
        label_obj->instances = realloc(label_obj->instances, sizeof(char*)*(label_obj->instances_num + 1));
        label_obj->instances[label_obj->instances_num] = *address;
        label_obj->instances_num++;
    }
}

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

void handle_entry_label(char* directive, char* line, int line_number){
    char* name_of_entry = NULL;
    char** address;
    move_line_ptr_to_next_word(directive, line);
    name_of_entry = getFirstWord(line);
    if (is_valid_entry(name_of_entry, line, line_number)) {
        address = push_single_ic(name_of_entry);
        add_label(name_of_entry, 1, address, line_number, 0);
    }

    free(name_of_entry);
}

int is_valid_entry( char* entry_word, char* line, int line_number) { 
    char* check_more_param = NULL;
    move_line_ptr_to_next_word(entry_word, line);
    check_more_param = getFirstWord(line);
    if (!(check_more_param[0] == '\0')) {
        handleError("too many params for entry", line_number);
    }
    else if (isDirective(entry_word)){
        handleError("invalid name. name of entry is like a directve", line_number);
    }
    else if (isInstruction(entry_word)){
        handleError("invalid name. name of entry is like a instruction", line_number);
    }
    else if (!can_word_be_valid_label(entry_word, line_number)) {
        handleError("invalid name. invalid entry name", line_number);
    }
    else if (is_label_exist(entry_word)){
        handleError("invalid name. a label with the same name exists already", line_number);
    }
    else {
        free(check_more_param);
        return 1;
    }

    free(check_more_param);
    return 0;
}

int is_black_list_empty() {
    return labels_black_list == NULL;
}

/* checks if label exists in labels table */
int is_label_exist(char* label) {
    int i;
    int size;
    char* new_str;
    size = strlen(label);
    new_str = (char*)calloc(size,sizeof(char));
    if (label[size-1] == ':'){
        strncpy(new_str,label,size-1);
        strcpy(label,new_str);
    }

    /* if labels is not empty */
    if (labels != NULL) {
        /* go over all labels in labels data */
        for (i = 0; i < num_labels; i++) {
            if (strcmp(label, labels[i].name) == 0) {
                return 1;
            }
        }
    }
    free(new_str);
    return 0;
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

void print_all_black_list_labels_left() {
    int i, j;
    /* if labels is not empty */
    if (labels_black_list != NULL) {
        /* go over all labels in labels data */
        for (i = 0; i < black_list_num; i++) {
            char label_error_msg[MAX_LINE_LENGTH*2] = "label does not exist - ";
            strcat(label_error_msg, labels_black_list[i].name);
            handleError(label_error_msg, labels_black_list[i].line_number);
            for (j = 0; j < labels_black_list[i].instances_num; j++) {
                printf("            %p : %s\n", labels_black_list[i].instances[j], labels_black_list[i].instances[j]);
            }
            
        }
    }
}

/* returns label data */
Label* getLabel(char* label) {
    int i;
    /* if labels is empty */
    if (!labels) {
        for (i = 0; i < num_labels; i++) {
            if (strcmp(label, labels[i].name) == 0) {
                return &labels[i];
            }
        }
    }

    return NULL;
}