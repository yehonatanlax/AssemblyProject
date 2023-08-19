#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../functions/functions.h"
#include "../labels/labels.h"
#include "../dc/dc.h"
#include "../ic/ic.h"
#include "../preprocessor/macro.h"
#include "../extern/extern.h"




void analyzeFile(char *filename, int* ic, int* dc) {
  FILE *fileReed; /*ptr to path of reed file*/
  char line[MAX_LINE_LENGTH];
  char *firstWord;
  int line_number = 0;
  fileReed = fopen(filename, "r");
  /* check for errors */
  if (fileReed == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  /* analyze each line in file */
  while (fgets(line, MAX_LINE_LENGTH, fileReed)) {
    line_number++;
    printf("\n%d  ============================================\n%s\n", line_number, line);
    if (isLineEmptyOrComment(line)) { /* If the line comment or empty */
      printf("line %d is a comment or empty line\n", line_number);
      continue;
    } 
    else {
      firstWord = getFirstWord(line);
      if (is_word_a_valid_label_declaration(firstWord, line_number)) { /* If the first word is a label */
        printf("Label: %s\n", firstWord);
        move_line_ptr_to_next_word(firstWord, line);/* Move line pointer to the next word in line */
        handle_command(line, firstWord, line_number);
      } 
      else {
        handle_command(line, NULL, line_number);
      }
    }
  }

  print_all_IC();
  print_labels();
}

/*=======================================================================================*/
/*=======================================================================================*/
/*=======================================================================================*/

