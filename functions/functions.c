
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

const char* directives[] = {".data", ".string", ".entry", ".extern"}; 
const char* ass_comm[] = {"mov","cmp", "add", "sub", "lea",
                          "not", "clr", "inc", "dec", "jmp", "bne", "red",
                          "prn", "jsr", "rts", "stop"};

/*======================================================================*/
/*
    remove spaces before content,
    This func for char[].
*/
void removeSpacesBefore(char *str) {
    int i, j;
    int len = strlen(str);
    /*if (*str == '\n') {
        return;
    }*/
    int nonSpaceFound = 0; /* Flag to track the first non-space character*/
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

int isLineEmptyOrComment(char* line) {
    /* I get line without spaces */
    int i;
    int size;
    size  = strlen(line);
    if (line[0] == ';'){
        return 1;
    }
    for (i = 0; i < size; i++){
        if (!isspace(line[i])){
            return 0;
        }
    }
    return 1;
}

int can_word_be_valid_label(char* word, int line_number) {
    int i;
    int size = strlen(word);
    if (size > MAX_LABEL_LENGTH + 1){/*If the word to much long */
        return 0;
    }
    if (!(isalpha(*word))){/*If the first char is not letter*/
        return 0;
    }
    /* check if the word is a saved instruction */
    if (isInstruction(word)) {
        handleError("you can't use saved word as a label", line_number);
        return 0;
    }
    for (i = 0; i < size; i++){ /*Check if all characters are valid*/
        if (!(isalpha(word[i]) || isdigit(word[i]))){
            return 0;
        }
    }

    return 1;
}

int is_word_a_valid_label_declaration(char* word, int line_number) {
    int size = strlen(word);
    char* withoutColon = (char*)malloc(size*sizeof(char));
    memset(withoutColon, '\0', size);
    strncpy(withoutColon, word, size-1);
    /* if label does not end with a colon this is not a declaration */
    if (word[size-1] == ':' && can_word_be_valid_label(withoutColon, line_number)){
        free(withoutColon);
        return 1;
    }

    free(withoutColon);
    return 0;
}

char* isDirective(char* word) {
    int i;
    for (i = 0; i < DIRECTIVES_NUM; i++) {
        if (!strcmp(word, directives[i])) {
            return word;
        }
    }
    return NULL;
}

char* isInstruction(char* word) {
    int i;
    for (i = 0; i < INSTRUCTIONS_NUM; i++) {
        if (!strcmp(word, ass_comm[i])) {
            return word;
        }
    }
    return NULL;
}

void move_line_ptr_to_next_word(char* word, char* line) { 
    int i = strlen(word);
    int j;
    removeSpacesBefore(line); /* remove spaces before word */
    for(j=0; line[i] != '\n' && line[i] != '\0'; i++, j++) {
        line[j] = line[i];
    }
    line[j] = '\0';
    removeSpacesBefore(line); /* remove spaces after word */
}

int remove_comma(char* line, int line_number) {
    removeSpacesBefore(line); /* remove spaces before expected comma */
    if (*line == ',') {
        strcpy(line, line+1);
    }
    else {
        handleError("missing comma", line_number);
        return 0;
    }
    removeSpacesBefore(line); /* remove spaces after comma */
    if (*line == ',') {
        handleError("more than one comma", line_number);
        return 0;
    }

    return 1;
}


char* encode_operand(char* operand) {
    /*printf("--- operand: %s\n", operand);*/
    return operand;
}

char* encode_two_regs(char* reg_src, char* reg_dst) {
    /*printf("--- operand: %s\n", reg_src);
    printf("--- operand: %s\n", reg_dst);*/
    return "registers";
}

/* handle directive line */
void handle_directive(char* instruction, char* line, char* label, int line_number) {
    if (!strcmp(".data", instruction)) {
        move_line_ptr_to_next_word(instruction, line);
        add_data(line, label, line_number);
    } 
    else if (!strcmp(".string", instruction)) {
        move_line_ptr_to_next_word(instruction, line);
        add_string(line, label, line_number);
    } 
    else if (!strcmp(".extern", instruction)) {
        handle_extern(instruction, line, line_number);
    } 
    else if (!strcmp(".entry", instruction)) {
        handle_entry_label(instruction, line, line_number);
    }
}

void add_line_to_ic(char** encoded_operands, char* instruction, char* label, int num_of_lines, int operands_num, int line_number, int src_type, int dst_type) {
    if (label == NULL || !is_label_exist(label)) { /* if label is given and already exist don't push */
        int op_types[2] = {src_type, dst_type};
        /* push instruction to IC data and save address in case of label */
        char** address = push_ic(encoded_operands, instruction, operands_num, op_types, line_number);
        /* if a label was written in this line, save the label with the current address */
        if (label) {
            add_label(label, num_of_lines, address, line_number, 1);
        }
    } 
    else {
        handleError("The name of this label already exists in labels table", line_number);
    }
}

void handle_instruction(char* instruction, char* line, char* label, int line_number) {
    char* operand_src;
    char* operand_dst;
    char* encoded_operands[2]; /* holds the binary representation of each operand */
    int operand_src_type, operand_dst_type = 0; /* types of operands */
    move_line_ptr_to_next_word(instruction, line); /* remove instruction from line */

    switch (instruction_params_num(instruction)) { /* handle instruction by number of params */
        case 0:
            /* push instruction to IC data */
            add_line_to_ic(encoded_operands, instruction, label, 1, 0, line_number, 0, 0);
            break;
        case 1:
            operand_dst = getFirstWord(line);
            operand_dst_type = type_of_operand(operand_dst, line_number);
            
            if (!operand_dst_type) { /* if invalid operand type */
                return;
            }
            if (strcmp("prn", instruction) != 0) { /* handle all cases that aren't "prn" */
                if (operand_dst_type == 1) {
                    handleError("type pf operand is invalid in case 1", line_number);
                    return;
                } 
                else {
                    /* replace operand with binary represintation */
                    encoded_operands[0] = encode_operand(operand_dst);
                }
            } 
            else {
                encoded_operands[0] = encode_operand(operand_dst); /* encode prn param */
            }

            /* push instruction to IC data */
            add_line_to_ic(encoded_operands, instruction, label, 2, 1, line_number, operand_dst_type, 0);
            break;
        case 2:
            operand_src = getFirstWord(line);
            operand_src_type = type_of_operand(operand_src, line_number);
            if (!operand_src_type) {
                return;
            }
            /* move line pointer to next word: expected a comma */
            move_line_ptr_to_next_word(operand_src, line);
            if (!remove_comma(line, line_number)) {
                return;
            }

            operand_dst = getFirstWord(line);
            operand_dst_type = type_of_operand(operand_dst, line_number);
            if (!operand_dst_type) { /* if invalid dest operand type */
                return;
            }

            /* check source operand */
            if (strcmp("lea", instruction) == 0) { /* handle "lea" instruction */
                if (operand_src_type != 3) {
                    handleError("invalid type of operand", line_number);
                    return;
                }
                else {
                    /* replace source operand with binary represintation in "lea" case */
                    encoded_operands[0] = encode_operand(operand_src);
                }
            }
            else {
                /* replace source operand with binary represintation */
                encoded_operands[0] = encode_operand(operand_src);
            }

            /* check if both are registers */
            if (operand_src_type == 5 && operand_dst_type == 5) {
                /* replace registers operands to one binary represintation */
                encoded_operands[0] = encode_two_regs(operand_src, operand_dst);
                /* push instruction to IC data */
                add_line_to_ic(encoded_operands, instruction, label, 2, 1, line_number, operand_src_type, operand_dst_type);
                break;
            }
            /* check dest operand */
            else if (strcmp("cmp", operand_dst) != 0) { /* handle all cases that aren't "cmp" */
                if (operand_dst_type == 1) {
                    handleError("invalid type of operand", line_number);
                    return;
                }
                else {
                    /* replace dest operand with binary represintation if not "cmp" case */
                    encoded_operands[1] = encode_operand(operand_dst);
                }
            }
            else {
                /* replace dest operand with binary represintation */
                encoded_operands[1] = encode_operand(operand_dst);
            }
            /* push instruction to IC data */
            add_line_to_ic(encoded_operands, instruction, label, 3, 2, line_number, operand_src_type, operand_dst_type);
            break;
    }

}

int is_number(char* s)
{
    int flag = 0; /* to check if we found at least one number */
    /* remove sign of number */
    if (*s == '-' || *s == '+') {
        s++;
    }
        
    while (*s != '\0')
    {
        if (isdigit(*s) == 0) {
            return 0;
        }
        flag = 1;
        s++;
    }

    return flag;
}

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

int type_of_operand(char* operand, int line_number) {
    /*char op;*/
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

void handleError(char* err, int line_number) {
    printf("Line %d: %s\n", line_number, err);
}

void handle_command(char * line, char* label, int line_number) {
    char* first_word = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    char* encoded_label = (char*)malloc(MAX_LINE_LENGTH*sizeof(char));
    char** address;
    first_word = getFirstWord(line);
    if (isDirective(first_word)) { /* If the word is a directive */
        printf("Directive: %s\n", first_word);
        handle_directive(first_word, line, label, line_number);
    } 
    else if (isInstruction(first_word)) { /* If the word is an instruction */
        printf("Instruction: %s\n", first_word);
        handle_instruction(first_word, line, label, line_number);
    } 
    else if (can_word_be_valid_label(first_word, line_number)) { /* if the word is a label */
        encoded_label = encode_operand(first_word);
        address = push_ic(NULL, encoded_label, 0, NULL, line_number);
        add_to_labels_black_list(first_word, line_number, address);
    }
    else  { /* If the second word is not a valid */
        handleError("invalid line - word is not an instruction and not a directive \n", line_number);
    }
}

char* getFirstWord(char * str){
    int i = 0;
    char * ptr = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
    removeSpacesBefore(str);
    if (*str == ',') {
        ptr[i] = ',';
        ptr[i+1] = '\0';
        return ptr;
    }
    while (!(isspace(str[i])) && str[i] != ',' && str[i] != '\0') {
        ptr[i] = str[i];
        i++;
    }
    ptr[i] = '\0';

    return ptr;
}

int count_word(char* line){
    int i = 0;
    int num_word = 0;
    while (line[i] != '\0'){
        if (isspace(line[i]) || line[i] == ','){
            i++;
            while (isspace(line[i]) || line[i] == ','){
                i++;
            }
            num_word++;
        }
        else{
            i++;
        }
    }
    return num_word;
}


