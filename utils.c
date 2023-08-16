#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#define BIN_LENGTH 12 
#define HALF_BIN_LEN 6
#define B_64_LEN 2

char memory[1024][12];/*TODO I dont know if we need it*/

const char* directives[] = {".data", ".string", ".entry", ".extern"}; 
const char* ass_comm[] = {"mov","cmp", "add", "sub", "lea",
                          "not", "clr", "inc", "dec", "jmp", "bne", "red",
                          "prn", "jsr", "rts", "stop"};


/*Splits the string of the bin-num and convert to base 64 string*/
/*TODO check is it legaL TO RETURN an inner string*/
char *analize_bin(char *bin_num){/*TODO we need to change the return, this is ilegal 
to return a local var, maybe we should print it*/
    int i = 0; 
    char first_bin[HALF_BIN_LEN +1];
    char second_bin[HALF_BIN_LEN +1];
    char first_sign;
    char second_sign;
    char full_sign[B_64_LEN];
    
    for(i = 0; i < BIN_LENGTH ; i++){
        if(i < HALF_BIN_LEN)
            first_bin[i] = bin_num[i];
        else
            second_bin[i- HALF_BIN_LEN] = bin_num[i];
    }
    first_bin[HALF_BIN_LEN] = 0;
    second_bin[HALF_BIN_LEN] = 0;

    first_sign = bin_to_b64_sign(first_bin);
    second_sign = bin_to_b64_sign(second_bin);

    full_sign[0] = first_sign; 
    full_sign[1] = second_sign; 
    full_sign[2] = 0;
    printf("%s\n", full_sign);

    return full_sign;
}

/*takes the half binary and convert it to a b64 sign*/
char bin_to_b64_sign(char *bin){
  int dec = bin_to_dec(bin);
  char sign = dec_to_b64(dec);
  return sign;
}

/*convert binary number to a decimal number*/
int bin_to_dec(char* dec){/*TODO add edge cases!!!!*/
  int i;
  int j = 0;
  int sum = 0;
  int length = arr_len(dec);
  for(i = length -1; i >= 0 ; i--){
    
    if (dec[i] == '1')
      sum += pow(2, j);
    j++;
  }
  return sum;
}


/*return the length of a string array*/
int arr_len(char* c){
  int j = 0;
  int length = 0;
  
  while(c[j] != 0){
    j++;
    length++;
  }
  
  return length; 
}
/*converting the decimal number to base 64 sign*/
char dec_to_b64(int num){
  char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  return base64[num];
}

/* This function creates a file name by appending suitable extension (by type) to the original string */
char *create_file_name(char *original, int format){
    char *name = (char *) malloc(strlen(original) + SUFFIX);
    if(name == NULL)
    {
        fprintf(stderr, "Dynamic allocation error.");
        exit(0);
    }

    strcpy(name, original);

    /* Concatenating the required suffix */

    switch (format)
    {
        case AM:
            strcat(name, ".as");
            break;
/*add other options*/

    }
    return name;
}

/*jumping the spaces in the begining of a line*/
char* space_jump(char* ch) {

    if(ch == NULL)
      return NULL;
    while (isspace(*ch)) 
      ch++;
    return ch;
}

void sec_pass(FILE *fp, char *filename){/*TODO need to movw to deifferent file*/
  char line[LINE_LENGTH];
  int num_of_line = 1;
  /*ic = 0; TODO/*In the rewind we should re-count the instruction*/
  /*TODO change the while loop to this: fgets(line, LINE_LENGTH, fp)!=NULL*/
  while(fgets(line, LINE_LENGTH, fp)!=NULL){/*the loop stops in the end of the file*/
    char *line_no_space;
    char word[LINE_LENGTH]; /*chank of letters from the line*/
    

    num_of_line++;
     printf("the line is %s", line);/*TODO-test*/ 
    line_no_space = space_jump(line);
    printf("the new line is %s", line_no_space);/*TODO-test*/

    parse_word(line_no_space, word);
    /*TODO making a func who parsing a chunk of letters*/



    
    
    
    
  }


}

char *parse_word(char *line, char *word){/*we need to take line no spaces*/
  int i;
  for(i = 0; line[i] != ' '; i++){
      word[i] = line[i];
  }
   word[i] = 0;/*adding the 'null' in the end*/
  return word;
}

char *write_binary_line(*temp_binary){
  int i = 0;
  char *code_type = {00, 01, 10};/*00 = abs., 01 = ext., 10 = reloc.*/
  /*note: we have the same arr in the header*/
  char *commands_binary[] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", 
            "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};

  char *adressing[] = {/*Immediate adressing*/ 001, /*Direcrt adressing*/ 011, /*Direct register adressing*/ 101};


  while(i<12){//initilize the string
    temp_binary[i] = '0';
    i++;
  }

  


  return temp_binary; 
}

/*Analize the beggining of the line and return 1 if the first word is a label*/
/*int isFirstWordLabel(char* word) {
    int i;
    int size = strlen(word);
    char* withoutColon;
    memset(withoutColon, '\0', size);
    strncpy(withoutColon, word, size-1);
    if (size > MAX_LABEL_LENGTH + 1){//If the word to much long
        return 0;
    }
    if (!(isalpha(word))){//If the first char is not letter
        return 0;
    }
    if (word[size-1] != ':'){
        return 0;
    }
    // check if the saved instruction 
    if (isInstruction(withoutColon)) {
        handleError("you can't use saved word as a label");
    }
    for (i = 0; i < size-1; i++){ /Check if all characters are valid/
        if (isalpha(word[i] || isdigit(word[i]))){
            return 0;
        }
    }
    return 1;
}*/


/*checks if a word in the line is a valid label*/
/*int isValidLabel(char * word){
    int i;
    int size = strlen(word);
    char * withoutColon;
    (void)strncmp(withoutColon, word, size-2);
    printf("withoutColon: %s\n",withoutColon);
    if (size > MAX_LABEL_LENGTH + 1){/*If the word to much long 
        return 0;
    }
    if (!(isalpha(word))){/*If the first char is not letter/
        return 0;
    }
    if (word[size-2] != ':'){
        return 0;
    }
    /* check if the saved instruction 
    if (isInstruction(withoutColon)) {
        handleError("you can't use saved word as a label");
    }
    for (i = 0; i < size-1; i++){ /Check if all characters are valid/
        if (isalpha(word[i] || isdigit(word[i]))){
            return 0;
        }
    }
    return 1;
}*/

/*check if word in the line is an instruction*/
/*char* isInstruction(char* word) {
    int i;
    for (i = 0; i < INSTRUCTIONS_NUM; i++) {
        if (!strcmp(word, ass_comm[i])) {
            return word;
        }
    }
    return NULL;
}*/