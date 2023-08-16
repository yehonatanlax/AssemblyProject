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

char** push_ic(char** operands, char* instruction, int operands_num, int* op_types, int line_number) {
  char** address;
  char** op_address;
  int i;
  /* save address of instruction in IC data in case of label  */
  address = push_single_ic(encode_operand(instruction));
  /* push all operands */
  for(i = 0; i < operands_num; i++) {
    op_address = push_single_ic(operands[i]);
    if (op_types[i] == 3) {
      add_to_labels_black_list(operands[i], line_number, op_address);
    }
  }
    
  return address;
}

/* push instruction / operand to IC data */
char** push_single_ic(char* ic) {
  /* allocate new space for the pointer */
  IC_data.ic_data = realloc(IC_data.ic_data, (IC_data.ic_counter + 1) * sizeof(char**));
  /* allocate new space for the data itself */  
  *(IC_data.ic_data + IC_data.ic_counter) = malloc(strlen(ic) + 1);
  strcpy(IC_data.ic_data[IC_data.ic_counter], ic);
  IC_data.ic_counter++; 
  printf("Pushed IC: %s\n", ic);
  /* return address of data in IC data table */
  return &IC_data.ic_data[IC_data.ic_counter - 1];
}

void print_all_IC() {
  int i, j;
  Label* label;
  printf("-------------------------------------------------------------------\n");
  printf("---------------aaaaaaaaaaaaaaaaaaaaaaaaaaaa-------------------------------\n");
  for (i = 0; i < IC_data.ic_counter; i++) {
    label = getLabel(IC_data.ic_data[i]);
    if (label != NULL) {
      for (j = 0; j < label->params_counter; j++) {
        printf("%s\n", (*(label->address)+j));
      }
    } else {
      printf("%s\n", IC_data.ic_data[i]);
    }
  }

  print_all_black_list_labels_left();

}




/* ------------------------------------------------------------------------ */
