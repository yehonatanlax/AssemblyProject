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

int line_number = 0;

/* goes over each line and handles the code given */
void analyzeFile(char *filename, int* ic, int* dc) {
  FILE *fileReed; /*ptr to path of reed file*/
  char line[MAX_LINE_LENGTH];
  char *firstWord;
  line_number = 0;
  fileReed = fopen(filename, "r");
  /* check for errors */
  if (fileReed == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  /* analyze each line in file */
  while (fgets(line, MAX_LINE_LENGTH, fileReed)) {
    line_number++;
    /* checks if the line is a comment or an empty line */
    if (is_line_empty_or_comment(line)) { /* If the line comment or empty */
      continue;
    } 
    else {
      /* checks the first word in line */
      firstWord = get_first_word(line);
      /* if it's a label */
      if (is_word_a_valid_label_declaration(firstWord, line_number)) {
        move_line_ptr_to_next_word(firstWord, line);/* Move line pointer to the next word in line */
        handle_command(line, firstWord, line_number);
      } 
      else { /* if ther's no label at the beginning */
        handle_command(line, NULL, line_number);
      }
    }
  }
}