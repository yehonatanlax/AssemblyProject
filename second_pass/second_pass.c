#include "second_pass.h"
#include "../dc/dc.h"
#include "../extern/extern.h"
#include "../functions/functions.h"
#include "../ic/ic.h"
#include "../labels/labels.h"
#include "../preprocessor/macro.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Blacked_Label *labels_black_list;
extern int black_list_num;

extern Label *labels;
extern int num_labels;

extern Extern *externs;
extern int num_externs;

extern IC IC_data;
extern int ic;

extern DC DC_data;
extern int dc;

extern int flag_err;
extern int line_number;

void second_pass(char* file_name) {
    int i;
    int j;
    int z;
    int x;
    int y;
    int t;
    int flag;
    int q;
    int w;
    int u;
    int address, counter;
    FILE* entry_file = NULL;
    FILE* extern_file = NULL;
    /* chaeck if have to much data */
    if ((IC_data.ic_counter + DC_data.dc_counter) > 923) {
        flag_err = 1;
    }

    for (w = 0; w < num_labels; w++) {
        if (labels[w].type != 1) {
            counter = 0;
            for (q = 0; q < DC_data.dc_counter; q++) {
                if (DC_data.dc_data[q].label != NULL && strcmp(DC_data.dc_data[q].label, labels[w].name) == 0) {
                    address = 100 + counter + IC_data.ic_counter;
                    labels[w].address = address;
                    break;
                }
                counter += DC_data.dc_data[q].num;
            }
        }
    }

    for (i = 0; i < black_list_num; i++) {
        flag = 0;
        for (j = 0; j < num_labels; j++) { /* check if exist in labels tabel */
            if (strcmp(labels_black_list[i].name, labels[j].name) == 0) {
                if (labels[j].type == 0) {         /* if it is "entry" */
                    if (labels[j].address == -1) { /* if entry does not define */
                        handleError("entry declard but not define", labels_black_list[i].line_number);
                        flag = 1;
                        break;
                    } 
                    else { /* if entry define, update all the instances to address of declaration */
                        char* entry_code = encode_binary(-1, -1, -1, 2, -1, -1, labels[j].address, NULL, line_number);
                        for (z = 0; z < labels_black_list[i].instances_num; z++) { /* update all the instances to address of declaration  */
                            IC_data.ic_data[labels_black_list[i].instances[z] - 100] = entry_code;
                            if (!flag_err){
                                if (entry_file == NULL) {
                                    entry_file = create_file("ent",file_name);
                                }
                                for (u = 0; u < num_labels; u++) {
                                    if (labels[u].type == 0) {
                                        add_to_file(entry_file,labels[u].name, labels[u].address);
                                    }
                                }
                            }
                            else {
                                remove_files(file_name);
                            }
                            
                        }
                        flag = 1;
                        break;
                    }
                } 
                else { /* if it is jsut "label" */
                    char* entry_code = encode_binary(-1, -1, -1, 2, -1, -1, labels[j].address, NULL, line_number);
                    for (x = 0; x < labels_black_list[i].instances_num; x++) {/* update all the instances to address of declaration  */
                        IC_data.ic_data[labels_black_list[i].instances[x] - 100] = entry_code;
                    }
                    flag = 1;
                    break;
                }
            } 
        }
        if (!flag) {
            for (y = 0; y < num_externs; y++) {
                if (strcmp(labels_black_list[i].name, externs[y].name) == 0) {
                    char *entry_code = encode_binary(-1, -1, -1, 1, -1, -1, -1, NULL, line_number);
                    flag = 1;
                    for (t = 0; t < labels_black_list[i].instances_num; t++) { /* update all the instances to address of declaration  */
                        IC_data.ic_data[labels_black_list[i].instances[t]-100] = entry_code;   
                        if (!flag_err){
                            if (extern_file == NULL) {
                                extern_file = create_file("ext", file_name);
                            }
                                add_to_file(extern_file,labels_black_list[i].name, labels_black_list[i].instances[t]);
                        }
                        else{
                            remove_files(file_name);
                        }
                    }
                }
            }
        }
        if (!flag) {
            printf("name of bl: %s line_num: %d\n", labels_black_list[i].name, labels_black_list[i].line_number);
            handleError("undefined label or operand", labels_black_list[i].line_number);
        }
    }
}

void create_ob_file(char* file_name) {
    FILE* file;
    int dc_counter = 0;
    int i,j;
    char* first_part = malloc(7*sizeof(char));
    char* second_part = malloc(7*sizeof(char));
    char* double_sign = malloc(3*sizeof(char));
    file = create_file("ob", file_name);
    for (i = 0; i < DC_data.dc_counter; i++) {
        dc_counter += DC_data.dc_data[i].num;
    }
    fprintf(file, "%d %d\n",IC_data.ic_counter, dc_counter);
    for (i = 0; i < IC_data.ic_counter; i++) {
        first_part = split_bin_to_first_part(IC_data.ic_data[i]);
        second_part = split_bin_to_second_part(IC_data.ic_data[i]);
        double_sign = bin_to_b64_sign(first_part, second_part);
        fprintf(file, "%s\n",double_sign);
    }
    for (i = 0; i < DC_data.dc_counter; i++) {
        for (j = 0; j < DC_data.dc_data[i].num; j++) {
            first_part = split_bin_to_first_part(DC_data.dc_data[i].data[j]);
            second_part = split_bin_to_second_part(DC_data.dc_data[i].data[j]);
            double_sign = bin_to_b64_sign(first_part, second_part);
            fprintf(file, "%s\n",double_sign);
        }
    }
}

FILE* create_file(char* kind_file, char* name_file) {
    FILE* file = malloc(sizeof(FILE));
    char* name;
    int size = strlen(name_file);
    name = malloc(size*sizeof(char)+4);
    strcpy(name, name_file);
    if (strcmp(kind_file,"ent") == 0) {
        strcat(name,".ent");
        file = fopen(name, "w");
    }
    else if (strcmp(kind_file,"ext") == 0) {
         strcat(name,".ext");
        file = fopen(name, "w"); 
    }
    else {
         strcat(name_file,".ob");
        file = fopen(name_file, "w");
    }
    if (file == NULL) {
        perror("Error opening file");
        return NULL; 
    }
    free(name);
    return file;
}

void add_to_file(FILE* file, char* name_label, int address) {
    fprintf(file, "%s\t%d\n",name_label, address);
}

void remove_files(char* name) {
    char* temp_name_ent;
    char* temp_name_ext;
    char* temp_name_obj;
    int size = strlen(name);
    temp_name_ent = malloc(size*sizeof(char)+4);
    temp_name_ext = malloc(size*sizeof(char)+4);
    temp_name_obj = malloc(size*sizeof(char)+4);
    strcpy(temp_name_ent, name);
    strcpy(temp_name_ext, name);
    strcpy(temp_name_obj, name);
    strcat(temp_name_ent, ".ent");
    strcat(temp_name_ext, ".ext");
    strcat(temp_name_obj, ".obj");
    remove(temp_name_ent);
    remove(temp_name_ext);
    remove(temp_name_obj);
}

