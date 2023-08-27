#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "functions.h"
#include "../labels/labels.h"
#include "../dc/dc.h"
#include "../ic/ic.h"
#include "../preprocessor/macro.h"
#include "../extern/extern.h"

#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 31
#define DIRECTIVES_NUM 4
#define INSTRUCTIONS_NUM 16

int flag_err = 0; /* if an eeror was found while processing the code */
char* DEFAULT_BINARY = "000000000000"; /* default binary code for addresses that are'nt known in first pass (like labels, externs etc. )*/
const char* directives[] = {".data", ".string", ".entry", ".extern"}; 
const char* ass_comm[] = {"mov","cmp", "add", "sub", "lea",
                          "not", "clr", "inc", "dec", "jmp", "bne", "red",
                          "prn", "jsr", "rts", "stop"};
/*======================================================================*/

/* remove spaces before content */
void remove_spaces_before(char *str) {
    int i, j;
    int len = strlen(str);
    int nonSpaceFound = 0; /* Flag to track the first non-space character */
    for (i = 0, j = 0; i < len; i++) {
        if (!isspace(str[i]) || nonSpaceFound) {
            str[j++] = str[i];
            nonSpaceFound = 1;
        }
    }

    /* Null-terminate the string at the new end*/
    if (nonSpaceFound) {
        str[j] = '\0';
    }
}

/* checks if given line is empty or a comment line */
int is_line_empty_or_comment(char* line) {
    int i;
    int size = strlen(line);
    /* comment case */
    if (line[0] == ';'){
        return 1;
    }

    /* check if all characters in line are space charachters */
    for (i = 0; i < size; i++){
        if (!isspace(line[i])){
            return 0;
        }
    }

    return 1;
}

/* checks if a given word could be a valid label name */
int can_word_be_valid_label(char* word, int line_number) {
    int i;
    int size = strlen(word);
    /* If the word is to much long */
    if (size > MAX_LABEL_LENGTH + 1){
        return 0;
    }

    /* If the first char is not a letter */
    if (!(isalpha(*word))){
        return 0;
    }

    /* check if the word is a saved instruction */
    if (is_instruction(word) != -1) {
        handleError("you can't use a saved word as a label", line_number);
        return 0;
    }

    /* Check if all characters are valid */ 
    for (i = 0; i < size; i++){ 
        if (!(isalpha(word[i]) || isdigit(word[i]))){
            return 0;
        }
    }

    return 1;
}

/* checks if a given word could be a valid label declaration */
int is_word_a_valid_label_declaration(char* word, int line_number) {
    int size = strlen(word);
    char* withoutColon = (char*)malloc(size*sizeof(char));
    memset(withoutColon, '\0', size);
    strncpy(withoutColon, word, size-1);

    /* if label does not end with a colon or word without colon can't be a valid label name - then this is not a label declaration */
    if (word[size-1] == ':' && can_word_be_valid_label(withoutColon, line_number)){
        free(withoutColon);
        return 1;
    }

    free(withoutColon);
    return 0;
}

/* checks if the given word is a directive */
char* is_directive(char* word) {
    int i;
    for (i = 0; i < DIRECTIVES_NUM; i++) {
        if (!strcmp(word, directives[i])) {
            return word;
        }
    }
    return NULL;
}

/* checks if th given word is a saved instruction */
int is_instruction(char* word) {
    int i;
    for (i = 0; i < INSTRUCTIONS_NUM; i++) {
        if (!strcmp(word, ass_comm[i])) {
            return i;
        }
    }
    return -1;
}

/* move line pointer to the first next non-space charachter */
void move_line_ptr_to_next_word(char* word, char* line) { 
    int i = strlen(word);
    int j;
    remove_spaces_before(line); /* remove spaces before word */
    /* copy each charachter to the beginning of the line */
    for(j = 0; line[i] != '\n' && line[i] != '\0'; i++, j++) {
        line[j] = line[i];
    }
    line[j] = '\0';
    remove_spaces_before(line); /* remove spaces after word */
}

/* remove first comma from the beginning of the line */
int remove_comma(char* line, int line_number) {
    int i,j;
    remove_spaces_before(line); /* remove spaces before expected comma */
    if (*line == ',') {
        for (i = 1, j = 0; i < strlen(line); i++, j++) {
            line[j] = line[i];
        }
        line[j] = '\0';
    }
    /* if there is no comma at the beginning of the line - error */
    else {
        handleError("missing comma", line_number);
        return 0;
    }
    remove_spaces_before(line); /* remove spaces after comma */
    /* if more than one comma */
    if (*line == ',') {
        handleError("more than one comma", line_number);
        return 0;
    }

    return 1;
}

/* handle directive line */
void handle_directive(char* directive, char* line, char* label, int line_number) {
    move_line_ptr_to_next_word(directive, line); /* remove directive declaration from line */
    /* data case */
    if (!strcmp(".data", directive)) {
        add_data(line, label, line_number);
    } 
    /* string case */
    else if (!strcmp(".string", directive)) {
        add_string(line, label, line_number);
    } 
    /* extern case */
    else if (!strcmp(".extern", directive)) {
        handle_extern(directive, line, line_number);
    } 
    /* entry case */
    else if (!strcmp(".entry", directive)) {
        handle_entry_label(directive, line, line_number);
    }
}

/* add line to IC data */
void add_line_to_ic(char** encoded_operands, char* instruction, char* label, int num_of_lines, int operands_num, int line_number, int src_type, int dst_type, char* op_to_bl) {
    if (label == NULL || !is_label_exist(label)) { /* if label is given and already exist don't push */
        int address;
        int op_types[2];
        /* set operand types */
        op_types[0] = src_type;
        op_types[1] = dst_type;
        /* push instruction to IC data and save address in case of label declaration in line */
        address = push_ic(encoded_operands, instruction, operands_num, op_types, line_number, op_to_bl);
        /* if a label was written in this line, save the label with the current address */
        if (label) {
            add_label(label, num_of_lines, address, line_number, 1);
        }
    } 
    else {
        handleError("The name of this label already exists in labels table", line_number);
    }
}

/* handle instruction line */
void handle_instruction(char* instruction, char* line, char* label, int line_number) {
    char* operand_src;
    char* operand_dst;
    char* encoded_operands[2]; /* holds the binary representation of each operand */
    int operand_src_type, operand_dst_type = 0; /* types of operands */
    move_line_ptr_to_next_word(instruction, line); /* remove instruction word from line */

    switch (instruction_params_num(instruction)) { /* handle instruction by number of params */
        /* case of instruction with no params */
        case 0:
            /* push instruction to IC data */
            add_line_to_ic(encoded_operands, instruction, label, 1, 0, line_number, -1, -1, NULL);
            break;
        /* case of instruction with 1 param */
        case 1:
            operand_dst = get_first_word(line);
            operand_dst_type = type_of_operand(operand_dst, line_number);
            
            if (!operand_dst_type) { /* if invalid operand type */
                return;
            }
            if (operand_dst_type == 1) { 
                /* only prn instruction cann have a param of type 1 */
                if (strcmp("prn", instruction) != 0) {
                    handleError("invalid type of operand is invalid", line_number);
                    return;
                }
                encoded_operands[0] = encode_binary(-1, -1, -1, 0, -1, -1, -1, operand_dst,line_number); /* encode param */
            }
            else if (operand_dst_type == 3) {
                encoded_operands[0] = DEFAULT_BINARY; /* label operand get's a default value in first pass */
            }
            else if (operand_dst_type == 5) {
                encoded_operands[0] = encode_binary(-1, -1, -1, 0, -1, get_register_number(operand_dst), -1, NULL, line_number); /* encode param */
            }

            /* push instruction to IC data */
            add_line_to_ic(encoded_operands, instruction, label, 2, 1, line_number, operand_dst_type ,-1, operand_dst);
            break;
        /* case of instruction with 2 params */
        case 2:
            operand_src = get_first_word(line);
            operand_src_type = type_of_operand(operand_src, line_number);
            if (!operand_src_type) { /* if invalid dest operand type */
                return;
            }
            /* move line pointer to next word: expected a comma */
            move_line_ptr_to_next_word(operand_src, line);
            if (!remove_comma(line, line_number)) {
                return;
            }

            operand_dst = get_first_word(line);
            operand_dst_type = type_of_operand(operand_dst, line_number);
            if (!operand_dst_type) { /* if invalid dest operand type */
                return;
            }

            if (strcmp("lea", instruction) == 0) { /* handle "lea" instruction */
                if (operand_src_type != 3) {
                    handleError("invalid type of operand: lea instruction cannot get this type of source operand", line_number);
                    return;
                }
            }

            if (strcmp("cmp", operand_dst) != 0) { /* handle all cases that aren't "cmp" */
                if (operand_dst_type == 1) {
                    handleError("invalid type of operand", line_number);
                    return;
                }
            }

            /* check if both - source and dest operands - are registers */
            if (operand_src_type == 5 && operand_dst_type == 5) {
                /* replace registers operands to one binary represintation */
                encoded_operands[0] = encode_binary(operand_src_type, -1, operand_dst_type, 0, get_register_number(operand_src), get_register_number(operand_dst), -1, NULL, line_number); /* encode param */
                /* push instruction to IC data */
                add_line_to_ic(encoded_operands, instruction, label, 2, 1, line_number, operand_src_type, operand_dst_type, operand_dst);
                return;
            }

            /* encode source operand */
            if (operand_src_type == 1) {
                encoded_operands[0] = encode_binary(operand_src_type, -1, -1, 0, -1, -1, -1, operand_src, line_number); /* encode param */
            }
            else if (operand_src_type == 3) {
                encoded_operands[0] = DEFAULT_BINARY;
            }
            else if (operand_src_type == 5) {
                encoded_operands[0] = encode_binary(operand_src_type, -1, -1, 0, get_register_number(operand_src), -1, -1, NULL, line_number); /* encode param */
            }

            /* replace dest operand with binary represintation */
            if (operand_dst_type == 1) {
            encoded_operands[1] = encode_binary(-1, -1, operand_dst_type, 0, -1, -1, -1, operand_dst, line_number); /* encode param */
            }
            else if (operand_dst_type == 3) {
                encoded_operands[1] = DEFAULT_BINARY;
            }
            else if (operand_dst_type == 5) {
                encoded_operands[1] = encode_binary(-1, -1, operand_dst_type, 0, -1, get_register_number(operand_dst), -1, NULL, line_number); /* encode param */
            }
            /* push instruction to IC data */
            add_line_to_ic(encoded_operands, instruction, label, 3, 2, line_number, operand_src_type, operand_dst_type, operand_dst);
            break;
    }

}

/* check if given string is a number */
int is_number(char* str)
{
    int flag = 0; /* to check if we found at least one number */
    /* remove sign of number */
    if (*str == '-' || *str == '+') {
        str++;
    }
        
    while (*str != '\0')
    {
        if (isdigit(*str) == 0) {
            return 0;
        }
        flag = 1;
        str++;
    }

    return flag;
}

/* check if word is a valid register */
int check_if_valid_register(char* reg, int line_number) {
    if (strlen(reg) == 2) {
        if (*reg == 'r') {
            reg++;
            if ((*reg - 48) >= 0 && (*reg - 48) <= 7) {
                return 1;
            }
        }
    }

    handleError("invalid register", line_number);
    return 0;
}

/* get register number */
int get_register_number(char* reg) {
    return *(reg+2) - 48;
}

/* get type of operand: 1, 3 or 5. returns 0 in case of invalid type */
int type_of_operand(char* operand, int line_number) {
    if (*operand == '@') { /* register case */
        operand++;
        if (check_if_valid_register(operand, line_number)) {
            return 5;
        }
        return 0;
    }
    if (is_number(operand)) { /* number case */
        return 1;
    }
    /* label case */
    if (can_word_be_valid_label(operand, line_number)) { /* if labels exist */
        return 3;
    } 
    handleError("inalid type of addressing method", line_number);
    return 0;
}

/* get number of params the instruction expects to get */
int instruction_params_num(char* instruction){
    if ((!strcmp(instruction,"mov")) || (!strcmp(instruction,"cmp")) ||(!strcmp(instruction,"add"))
    || (!strcmp(instruction,"sub")) ||(!strcmp(instruction,"lea"))){
        return 2;
    }
    else if ((!strcmp(instruction,"stop")) || (!strcmp(instruction,"rts"))){
        return 0;
    }
    return 1;
}

/* handle errors: prints number of line + the error */
void handleError(char* err, int line_number) {
    flag_err = 1; /* signs error was found in file*/
}

/* handle's a command line from file */
void handle_command(char * line, char* label, int line_number) {
    char* first_word = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    int address;
    first_word = get_first_word(line); /* get first word in line */
    if (is_directive(first_word)) { /* If the word is a directive */
        handle_directive(first_word, line, label, line_number);
    } 
    else if (is_instruction(first_word) != -1) { /* If the word is an instruction */
        handle_instruction(first_word, line, label, line_number);
    } 
    else if (can_word_be_valid_label(first_word, line_number)) { /* if the word is a label */
        address = push_single_ic(DEFAULT_BINARY);
        add_to_labels_black_list(first_word, line_number, address);
    }
    else  {
        handleError("invalid line - word is not an instruction/directive/label \n", line_number);
    }
}

/* get first word in given line */
char* get_first_word(char * str){
    int i = 0;
    char * ptr = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
    remove_spaces_before(str);
    /* in case the first word is a comma */
    if (*str == ',') {
        ptr[i] = ',';
        ptr[i+1] = '\0';
        return ptr;
    }
    while (!(isspace(str[i])) && str[i] != ',' && str[i] != '\0') {
        if (str[i] == ':') {
            ptr[i] = str[i];
            i++;
            break;
        }
        ptr[i] = str[i];
        i++;
    }
    ptr[i] = '\0';

    return ptr;
}

/* clear error flag */
void clear_error_flag() {
    flag_err = 0;
}


