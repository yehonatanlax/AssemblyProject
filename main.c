#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "functions/functions.h"
#include "labels/labels.h"
#include "dc/dc.h"
#include "ic/ic.h"
#include "preprocessor/macro.h"
#include "extern/extern.h"
#include "first_pass/first_pass.h"
#include "second_pass/second_pass.h"

int dc = 0;
int ic = 0;
extern int flag_err;

int main(int argc, char * argv[]) {
  int i;
  int line_number = 1;
  char *filepath = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
  char *name_file = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
 
  if (argc <= 1){
    perror("missing arguments in command line");
    exit(1);
  }
  for(i = 1; i < argc; i++) {
    initialize_extern();
    initialize_dc();
    initialize_ic();
    initialize_labels();
    clear_error_flag();
    memset(filepath, '\0', strlen(argv[i])+1);
    strcpy(filepath,argv[i]);
    strcat(filepath, ".as");
    filepath = (char*)preprocessor(filepath, line_number);
    analyzeFile(filepath,&ic, &dc);
    memset(name_file, '\0', strlen(filepath)+1);
    strncpy(name_file, filepath, (strlen(filepath) - 3));
    second_pass(name_file);
    if (!flag_err) {
      create_ob_file(name_file);
    }
    free_memory_dc();
    free_memory_extern();
    free_memory_ic();
    free_memory_label();
    free_memory_macro();
  }
  free(filepath);
  free(name_file);

  return 0;
}
