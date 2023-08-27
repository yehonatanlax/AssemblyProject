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

/* push all line data to ic data */
int push_ic(char** operands, char* instruction, int operands_num, int* op_types, int line_number, char* op_to_bl) {
  int address;
  int op_address[2]; /* will hold src and dst address */
  int i;

  /* push instruction */
  /* case of instruction with 2 registers */
  if (op_types[1] == 5 && op_types[0] == 5) {
    address = push_single_ic(encode_binary(op_types[0], is_instruction(instruction), op_types[1], 0, -1, -1, -1, NULL, line_number));
  }
  /* case of instruction with only one param */
  else if (operands_num == 1) {
    address = push_single_ic(encode_binary(-1, is_instruction(instruction), op_types[0], 0, -1, -1, -1, NULL, line_number));
  }
  /* all other cases */
  else {
    address = push_single_ic(encode_binary(op_types[0], is_instruction(instruction), op_types[1], 0, -1, -1, -1, NULL, line_number));
  }

  /* push operands */
  /* push each operand */
  for(i = 0; i < operands_num; i++) {
    op_address[i] = push_single_ic(operands[i]);
    if (op_types[i] == 3) {
      add_to_labels_black_list(op_to_bl, line_number, op_address[i]);
    }
  }

  return address;
}

/* push binary code to IC data */
int push_single_ic(char* encoded_ic) {
  /* allocate new space for the pointer */
  IC_data.ic_data = realloc(IC_data.ic_data, (IC_data.ic_counter + 1) * sizeof(char**));
  /* allocate new space for the data itself */  
  IC_data.ic_data[IC_data.ic_counter] = malloc(13*sizeof(char));
  strcpy(IC_data.ic_data[IC_data.ic_counter], encoded_ic);
  IC_data.ic_counter++;  
  /* return address of data in IC data table (starting from address 100)*/
  return IC_data.ic_counter + 99;
}

/* free memory of IC data */
void free_memory_ic() {
  free(IC_data.ic_data);
}

/* initialize data of IC */
void initialize_ic() {
  IC_data.ic_data = NULL;
  IC_data.ic_counter = 0;
}
