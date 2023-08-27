#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../functions/functions.h"
#include "macro.h"
#include "../labels/labels.h"
#include "../dc/dc.h"
#include "../ic/ic.h"
#include "../preprocessor/macro.h"
#include "../extern/extern.h"


#define MAX_LINE_LENGTH 80

Macro* macros = NULL; /*Array pointers of macros*/
int num_macros = 0;

/* this functions finds all macros declarations*/
char* preprocessor(const char* filename, int line_number) {
    char line[MAX_LINE_LENGTH];
    char* name = NULL;
    char* content = NULL;
    int inside_macro = 0;
    int i,j;
   
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_LINE_LENGTH, file)) {
            remove_spaces_before(line);
        if (strncmp(line, "mcro", 4) == 0) {
            name = malloc(strlen(line) - 3); 
            for (i = 5, j = 0; *(line+i) != '\n'; i++) {/*copy the name of mcaro without the "mcro" and spaces characters*/
                if (!isspace(*(line+i))) {
                    name[j++] = line[i];
                }
            }
            inside_macro = 1;
        } 
        else if (strncmp(line, "endmcro", 7) == 0) {
            if (inside_macro && name != NULL) {
                add_macro(name, content, line_number);
                name = NULL;/*Cleening the variable "name" for the next macro*/
                content = NULL;/*Cleening the variable "content" for the next macro*/
                inside_macro = 0;
            }
        } 
        else if (inside_macro && name != NULL) {
            if (content == NULL) {
                content = malloc(strlen(line) + 1);
                *content = '\0';
            }
            content = realloc(content, strlen(content) + strlen(line) + 1);
            strcat(content, line);
        }
    }
    free(name);
    free(content);
    fclose(file);
    return replace_macro_content(filename);
}

/* ----------------------------------second pass of preprocessor----------------------------------*/
char* replace_macro_content(const char* filepath) {
    char line[MAX_LINE_LENGTH];
    char updated_content[MAX_LINE_LENGTH]= "";
    int i;
    int len;
    char isMacro[MAX_LINE_LENGTH];
    int inside_macro = 0;
    char* change_extension_file = (char*)malloc(strlen(filepath) * sizeof(char));
    FILE* fileWrite; /*ptr to path of write file*/
    FILE* fileReed; /*ptr to path of reed file*/

    fileReed = fopen(filepath, "r");
    if (fileReed == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    len = strlen(filepath);
    strncpy(change_extension_file, filepath, len-3  );
    change_extension_file[len-3] = '\0';
    strcat(change_extension_file,".am");
    fileWrite = fopen(change_extension_file, "w");
    if (fileWrite == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_LINE_LENGTH, fileReed)) {
        char * first_word;
        remove_spaces_before(line);
        first_word = get_first_word(line);
        strcpy(isMacro,checkIfItsMacro(first_word));/* take the first word to avoid errors of spaces after the name of the macro*/
        if (inside_macro && (strncmp(line, "endmcro", 7) == 0)){
            inside_macro = 0;
            continue;
        }
        else if(inside_macro){
            continue;
        }
        else if ((strncmp(line, "mcro", 4) == 0)){
            inside_macro = 1;
            continue;
        }
        else if (strcmp(isMacro,"null") != 0) {
            for (i = 0; i < num_macros; i++){
                if (strcmp(isMacro, (macros)[i].name) == 0){
                    strcpy(updated_content, (macros)[i].content);
                    fputs(updated_content, fileWrite);
                    continue;
                }
            }
        }
        else {
            strcpy(updated_content, line);
            fputs(updated_content, fileWrite);

        }
    }

    fclose(fileReed);
    fclose(fileWrite);

    return change_extension_file;
}

void add_macro(char* name, char* content, int line_number) {
    if ((is_directive(name))){
        handleError("name of macro not valid dir", line_number);
        return;
    }
    if (is_instruction(name) != -1){
        handleError("name of macro not valid inst", line_number);
        return;
    }
    macros = realloc(macros, (num_macros + 1) * sizeof(Macro));/*Adding location for the new macro*/
    (macros)[num_macros].name = malloc(strlen(name) + 1);
    (macros)[num_macros].content = malloc(strlen(content) + 1);
    strcpy((macros)[num_macros].name, name);
    strcpy((macros)[num_macros].content, content);
    
    (num_macros)++;
}

/* This func checks if in the current line have call to macro */
char* checkIfItsMacro(char * word){
    int i,size;
    char * str = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
    if (str == NULL) {
        perror("Memory allocation failed in checkIfItsMacro.\n");
        return NULL;
    }
    if (num_macros == 0) {
        strcpy(str,"null");
    }
    for (i = 0; i < num_macros; i++) {
        /*Know where the end-of-string character is,
         to remove it because it interferes with the comparison*/
        size = strcspn(word, "\n");
        if (strncmp(word, (macros)[i].name,size) == 0){
            strcpy(str,(macros)[i].name) ;
            break;
        }
    strcpy(str,"null");/*If don't have match return null*/
    }
    return str;
}

void free_memory_macro() {
    free(macros);
}

void initialize_macro() {
    num_macros = 0;
    macros = NULL; /*Array pointers of macros*/
}
