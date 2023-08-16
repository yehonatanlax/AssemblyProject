#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dc.h"
#include "../labels/labels.h"
#include "../functions/functions.h"

DC DC_data = {NULL, 0};

/* add string of .string directive */
void add_string(char* str, char* label, int line_number){
    char** address;
    int i, flag;
    char new_str[MAX_LINE_LENGTH];
    flag = 0; /* flag is for checking if closing apostrophes of string exist */
    if (*str != '"') { /* if first char is not a string opening char */
        handleError("string expected but got invalid charachter", line_number);
        return;
    }
    else {
        str++;
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
                handleError("string expected but got invalid charachter", line_number);
                return;
            }
            str++;
        }
    }

    /* if flag is true then push string to data table */
    if (flag) {
        if (label == NULL || !is_label_exist(label)) { /* if label is given and already exist don't push */
            address = push_str(new_str);
            printf(" string added: %s\n", new_str);
            /* if a label was written in this line, save the label with the current address */
            if (label) {
                add_label(label, strlen(new_str), address, line_number, 1);
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
    int i;
    int number_found = 0; /* flag that represents if a number was found in the last iteration */
    char* numbers[MAX_LINE_LENGTH]; /* all numbers found in line */
    char** address; /* address of this directive in data */
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
        if (label == NULL || !is_label_exist(label)) { /* if label is given and already exist don't push */
            /* save address of first number in data in case of label */
            address = push_str(numbers[0]);
            printf(" number added: %s\n", numbers[0]);
            /* push all the rest numbers */
            for (i = 1; i < counter; i++) {
                push_str(numbers[i]);  
                printf(" number added: %s\n", numbers[i]);
            }
            /* if a label was written in this line, save the label with the current address */
            if (label) {
                add_label(label, counter, address, line_number, 1);
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

/* push string to data table */
char** push_str(char* str) {
    /* allocate new space for the string pointer */
    DC_data.dc_data = realloc(DC_data.dc_data, (DC_data.dc_counter + 1) * sizeof(char*));
    /* allocate new space for the string itself */
    *(DC_data.dc_data + DC_data.dc_counter) = malloc(strlen(str) + 1);
    strcpy(DC_data.dc_data[DC_data.dc_counter], str);
    DC_data.dc_counter++; 
    /* return address of string in data table */
    return &DC_data.dc_data[DC_data.dc_counter - 1];
}