#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "../preprocessor/macro.h"
#include "../first_pass/first_pass.h"


int main(int argc, char * argv[]) {
  int i;
  int ic = 0;
  int dc = 0;
  int line_number = 1;
  char *filepath = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
 
  if (argc <= 1){
  printf("%d",argc);
    perror("missing arguments in command line");
    exit(1);
  }
  /* TODO: FOR ALL THE FILES */
  filepath = argv[1];
  strcat(filepath, ".as");
  filepath = (char*)preprocessor(filepath, line_number);
  line_number = 0;
  analyzeFile(filepath,&ic, &dc);

  return 0;
}
