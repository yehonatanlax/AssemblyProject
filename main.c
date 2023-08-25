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

    strcpy(filepath,argv[i]);
    strcat(filepath, ".as");
    printf("ffffile_path: %s\n",filepath);
    printf("argv[i]: %s\n",argv[i]);
    printf("argv[2]: %s\n",argv[2]);
    filepath = (char*)preprocessor(filepath, line_number);
    printf("after preproccesor - ffffile_path: %s\n",filepath);
    analyzeFile(filepath,&ic, &dc);
    strncpy(name_file, filepath, (strlen(filepath) - 3));
    printf("nnnnnnn name_file: %s\n", name_file);
    second_pass(name_file);
    if (!flag_err) {
      create_ob_file(name_file);
    }
    /* TODO: DELETE THESE FUNCTIONS */
    get_externs();
    get_black_list();
    print_labels();
    print_all_IC();
    print_all_DC();

    free_memory_dc();
    free_memory_extern();
    free_memory_ic();
    /*free_memory_label();*/
    free_memory_macro();
  }

  return 0;
}
