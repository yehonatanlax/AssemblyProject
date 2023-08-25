#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dc.h"
#include "../ic/ic.h"
#include "../labels/labels.h"
#include "../functions/functions.h"

DC DC_data = {NULL, 0};
extern IC IC_data;

/* add string to DC of .string directive */
void add_string(char* str, char* label, int line_number){
    int i, flag;
    char* new_str = malloc(MAX_LINE_LENGTH*sizeof(char));
    flag = 0; /* flag is for checking if closing apostrophes of string exist */
    if (*str != '"') { /* if first char is not a string opening char */
        handleError("string expected but got invalid charachter", line_number);
        return;
    }
    else {
        str++;
        /* copy string content */
        for(i = 0; *str != '\0'; i++) {
            new_str[i] = *str;
            str++;
            /* if reached to end of string */
            if (*str == '"') {
                flag = 1;
                str++;
                break;
            }
        }

        new_str[++i] = '\0';

        /* checks if other charachters exist in line after string definition */
        while (*str != '\0') {
            if (*str != ' ') {
                handleError("invalid charachters after string value", line_number);
                return;
            }

            str++;
        }
    }

    /* if flag is true then string is valid and can be pushed to data table */
    if (flag) {
        if (label == NULL || !is_label_exist(label)) { /* if label is given and already exist don't push */
            push_data(&new_str, strlen(new_str)+1, line_number, 0, label);
            printf(" string added: %s\n", new_str);

            /* if this line is a label definition - save the label to labels table with an undefined address
                that will be initialized in the second pass after all ic data will be loaded */
            if (label) {
                add_label(label, strlen(new_str)+1, -1, line_number, 2);
            }
        }
        else {
            handleError("The name of this label already exists in labels table", line_number);
        }
    } 
    else {
        handleError("missing closing apostrophes of string", line_number);
        return;
    }
}

/* add data of .data directive */
void add_data(char* str, char* label, int line_number) {
    int number_found = 0; /* flag that represents if a number was found in the last iteration */
    char* numbers[MAX_LINE_LENGTH]; /* all numbers found in line */
    int counter = 0; /* num of numbers in line */
    while (*str != '\0') {
        char* num = getFirstWord(str);
        /* if a number needs to be found in this iteration and a number is found */
        if (!number_found && is_number(num)) {
            numbers[counter] = num;
            counter++; 
            move_line_ptr_to_next_word(num, str);
            number_found = 1;
        } 
        /* if a number was found in last iteration, now we excpect a comma */
        else if (number_found && !strcmp(num, ",")) {
            move_line_ptr_to_next_word(num, str);
            number_found = 0;
        }
        else {
            handleError("invalid charachter or missing comma between numbers", line_number);
            return;
        }
    }

    /* if numbers were found and the last iteration found a number (and not a comma) - add to data */
    if (number_found) {
        if (label == NULL || !is_label_exist(label)) { /* if the line is a label declaration and that label already exist don't push */
            push_data(numbers, counter, line_number, 1, label);
            
            /* if this line is a label definition - save the label to labels table with an undefined address
                that will be initialized in the second pass after all ic data will be loaded */
            if (label) {
                add_label(label, counter, -1, line_number, 2);
            }
        }
        else {
            handleError("The name of this label already exists in labels table", line_number);
        }
    }
    else {
        handleError("No numbers were found after .data directive", line_number);
    }
}

/* add to data table */
int push_data(char** data, int num, int line_number, int flag, char* label) {
    int i;
    char temp_buffer[2]; // will hold chars of string data
    char* encode = malloc(12*sizeof(char));
    /* allocate memory */
    DC_data.dc_data = realloc(DC_data.dc_data, (DC_data.dc_counter + 1) * sizeof(DC_obj));
    DC_data.dc_data[DC_data.dc_counter].data = malloc(num*sizeof(char*));
    DC_data.dc_data[DC_data.dc_counter].num = num;
    DC_data.dc_data[DC_data.dc_counter].label = NULL;

    /* if this line is a label declaration - add the label name to this data object */
    if (label) {
        DC_data.dc_data[DC_data.dc_counter].label = malloc(strlen(label)*sizeof(char));
        strcpy(DC_data.dc_data[DC_data.dc_counter].label, label);
    }
    
    /* go over all data array of this given data object */
    for (i = 0; i < num; i++){
        /* if flag is on - we are in data mode: adding numbers */
        if (flag) {
            encode = encode_binary(-1, -1, -1, -1, -1, -1, -1, data[i],line_number);
        }
        else { /* if flag is off - we are in string mode: adding each char separatly */
            temp_buffer[0] = data[0][i];
            temp_buffer[1] = '\0';
            encode = encode_binary(-1, -1, -1, -1, -1, -1, -1, temp_buffer,line_number);
        }

        /* set encoded data in object */
        DC_data.dc_data[DC_data.dc_counter].data[i] = malloc(strlen(encode)*sizeof(char));
        strcpy(DC_data.dc_data[DC_data.dc_counter].data[i], encode);
    }

    DC_data.dc_counter++; 
    
    /* return address of data object in data table */
    return DC_data.dc_counter - 1 ;
}

/* TODO: REMOVE FUNCTION */
void print_all_DC() {
  int i, j;
  printf("-------------------------------------------------------------------\n");
  printf("---------------print all dc-------------------------------\n");
  for (i = 0; i < DC_data.dc_counter; i++) {
    /*//label = getLabel(DC_data.dc_data[i].label);
    // if (label != NULL) {
    //   for (j = 0; j < label->params_counter; j++) {
    //     printf("%s\n", DC_data.dc_data[label->address + j]);
    //   }
    // } else {*/
    for (j =0; j < DC_data.dc_data[i].num; j++) {
      printf("%s\n", DC_data.dc_data[i].data[j]);
    }
  }
}

/* free memory allocation */
void free_memory_dc(){
    free(DC_data.dc_data);
}

/* initialize DC object */
void initialize_dc() {
    DC_data.dc_data = NULL;
    DC_data.dc_counter = 0;
}
