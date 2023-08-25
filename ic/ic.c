#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "ic.h"
#include "../labels/labels.h"
#include "../dc/dc.h"
#include "../functions/functions.h"

IC IC_data = {NULL, 0};

int push_ic(char** operands, char* instruction, int operands_num, int* op_types, int line_number, char* op_to_bl) {
  int address;
  int op_address[2];
  int i;
  /* save address of instruction in IC data in case of label  */
  if (op_types[1] == 5 && op_types[0] == 5) {
  /* handle 2 regs */
    address = push_single_ic(encode_binary(op_types[0], isInstruction(instruction), op_types[1], 0, -1, -1, -1, NULL, line_number));
  }
  else if (operands_num == 1) {
    address = push_single_ic(encode_binary(-1, isInstruction(instruction), op_types[0], 0, -1, -1, -1, NULL, line_number));
  }
  else{
    address = push_single_ic(encode_binary(op_types[0], isInstruction(instruction), op_types[1], 0, -1, -1, -1, NULL, line_number));
    }
    /* push all operands */
    for(i = 0; i < operands_num; i++) {
      op_address[i] = push_single_ic(operands[i]);
      if (op_types[i] == 3) {
        add_to_labels_black_list(op_to_bl, line_number, op_address[i]);
      }
  }

    
  return address;
}

/* push instruction / operand to IC data */
int push_single_ic(char* encoded_ic) {
  /* allocate new space for the pointer */
  IC_data.ic_data = realloc(IC_data.ic_data, (IC_data.ic_counter + 1) * sizeof(char**));
  /* allocate new space for the data itself */  
  IC_data.ic_data[IC_data.ic_counter] = malloc(13*sizeof(char));
  strcpy(IC_data.ic_data[IC_data.ic_counter], encoded_ic);
  IC_data.ic_counter++; 
  printf("Pushed IC: %s\n", IC_data.ic_data[IC_data.ic_counter - 1]);
  /* return address of data in IC data table */
  return IC_data.ic_counter + 99;
}

void print_all_IC() {
  int i, j;
  Label* label;
  printf("-------------------------------------------------------------------\n");
  printf("---------------print all ic-------------------------------\n");
  for (i = 0; i < IC_data.ic_counter; i++) {
    label = getLabel(IC_data.ic_data[i]);
    if (label != NULL) {
      for (j = 0; j < label->params_counter; j++) {
        printf("%s\n", IC_data.ic_data[label->address - 100 + j]);
      }
    } else {
      printf("%s\n", IC_data.ic_data[i]);
    }
  }
}

void free_memory_ic() {
  free(IC_data.ic_data);
}

void initialize_ic() {
  IC_data.ic_data = NULL;
  IC_data.ic_counter = 0;
}




/* ------------------------------------------------------------------------ */
