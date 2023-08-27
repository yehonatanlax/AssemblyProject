#include "encode.h"
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



/*takes the half binary and convert it to a b64 sign*/
char *bin_to_b64_sign(char *first_part, char *second_part){
int dec1, dec2;
char sign1, sign2;
char *double_sign = malloc(2*sizeof(char));
  dec1 = bin_to_dec(first_part);
  sign1 = dec_to_b64(dec1);
  dec2 = bin_to_dec(second_part);
  sign2 = dec_to_b64(dec2);
  double_sign[0] = sign1;
  double_sign[1] = sign2;

  return double_sign;
}

/*convert binary number to a decimal number*/
int bin_to_dec(char* dec){
  int i;
  int j = 0;
  int sum = 0;
  for(i = strlen(dec)-1; i >= 0 ; i--){
    
    if (dec[i] == '1')
      sum += pow(2, j);
    j++;
  }
  return sum;
}

/*converting the decimal number to base 64 sign*/
char dec_to_b64(int num){
  char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  return base64[num];
}

char *split_bin_to_first_part(char *bin_num){
    char* first_part = malloc(7*sizeof(char));
    int i;
    /*writing the first part*/
    for(i=0; i < 6;i++){
        first_part[i] = bin_num[i];
    }
    first_part[6] = 0;
    return first_part;
}

char *split_bin_to_second_part(char *bin_num){
    char* second_part = malloc(7*sizeof(char));
    int i;
    /*writing the second part*/
    for(i=0; i < 6;i++){
        second_part[i] = bin_num[i+6];
    }
    second_part[6] = '\0';
    return second_part;
}

char *encode_binary(int src_type, int insruction, int dst_type, int a_r_e, int src_reg, int dst_reg, int address, char *number, int line_number) {
  char *str, *bin_src_type, *bin_insruction, *bin_dst_type, *bin_a_r_e = NULL;
  int i;

  /*Error tester*/ 
   if (encode_binary_tester(src_type, insruction, dst_type, a_r_e, src_reg, dst_reg, address, number, line_number)){
    return "shshshshshs";
   }
  /*End of tester*/

  str = (char *)malloc(BIN_LINE * sizeof(char)); /*Allocate memory for a 12-character string*/
  if (str == NULL) {
    return "Error in allocating";
  }

  /*Initilize the memory*/
  for (i = 0; i < BIN_LINE - 1; i++) {
    str[i] = '0';
  }
  str[BIN_LINE - 1] = '\0'; /*The NULL term.*/

  /*If ARE is 01 there will be a constant binary number*/
  if (a_r_e == 1) {
    str[BIN_LINE - 2] = '1';
    return str;
  }

  /*check if we need to encode register*/
  /*when there is only src_reg*/
  if (src_reg != -1) {
    if (dst_reg == -1) {
      char *bin;
      bin = dec_to_bin(src_reg, line_number);
      for (i = 1; i < 5; i++)
        str[i] = bin[i - 1];
      /*adding ARE*/
      create_a_r_e(str, a_r_e, bin_a_r_e, line_number);
      return str;
    }
  }

  /*when there is only dst_reg*/
  if (dst_reg != -1) {
    if (src_reg == -1) {
      char *bin;
      bin = dec_to_bin(dst_reg, line_number);
      for (i = 6; i < 10; i++)
        str[i] = bin[i - 6];
      /*adding ARE*/
      create_a_r_e(str, a_r_e, bin_a_r_e, line_number);
      return str;
    }
  }

  /*when there are two registers*/
  if (src_reg != -1) {
    if (dst_reg != -1) {
      char *bin1;
      char *bin2;
      bin1 = dec_to_bin(src_reg, line_number);
      bin2 = dec_to_bin(dst_reg, line_number);
      for (i = 1; i < 5; i++)
        str[i] = bin1[i - 1];
      for (i = 6; i < 10; i++)
        str[i] = bin2[i - 6];
      /*adding ARE*/
      create_a_r_e(str, a_r_e, bin_a_r_e, line_number);
      return str;
    }
  }

   /*whan there are no registers (opcodes: rst, stop)*/
  if (src_reg == -1) {
    if (dst_reg == -1) {
      if (address == -1 && number == NULL) {
        switch (insruction) {
        case 14: {
          /*create insruction*/
          bin_insruction = dec_to_bin(insruction, line_number);
          for (i = 0; i < 4; i++) /*not using strncat because it adds NULL*/
            str[i + 3] = bin_insruction[i];
          return str;
          break;
        }
        case 15: {
          /*create insruction*/
          bin_insruction = dec_to_bin(insruction, line_number);
          for (i = 0; i < 4; i++) /*not using strncat because it adds NULL*/
            str[i + 3] = bin_insruction[i];
          return str;
          break;
        }
        }
      }
    }
  }

  /*encode_label*/
  if (a_r_e == 2 && address != -1) {
    bin_address(str, address, line_number);
    /*adding ARE*/
    create_a_r_e(str, a_r_e, bin_a_r_e, line_number);
    return str;
  }

  /*encode data or string or number*/
  data_str_or_num_to_bin(str, a_r_e, number, line_number);

  /*create src_type*/
  if (src_type != -1) {
    bin_src_type = dec_to_bin(src_type, line_number);
    bin_src_type++;         /*ignore the first letter in the four letters bin.*/
    for (i = 0; i < 3; i++) /*not using strncat because it adds NULL*/
      str[i] = bin_src_type[i];
  }

  /*create insruction*/
  if (insruction != -1) {
    bin_insruction = dec_to_bin(insruction, line_number);
    for (i = 0; i < 4; i++) /*not using strncat because it adds NULL*/
      str[i + 3] = bin_insruction[i];
  }

  /*create dst_type*/
  if (dst_type != -1) {
    bin_dst_type = dec_to_bin(dst_type, line_number);
    bin_dst_type++;         /*ignore the first letter in the four letters bin.*/
    for (i = 0; i < 3; i++) /*not using strncat because it adds NULL*/
      str[i + 7] = bin_dst_type[i];
  }

  /*create ARE*/
  if (a_r_e != -1)
    create_a_r_e(str, a_r_e, bin_a_r_e, line_number);

  return str;
}
/* ==================================== end of main func ========================= */
/* ==================================== end of main func ========================= */

/*gets the decimal rep. of instruction or addressing and
            convert to binary*/
char *dec_to_bin(int num, int line_number) {

  switch (num) {
  case 0:
    return "0000"; 
    break;
  case 1:
    return "0001";
    break;
  case 2:
    return "0010";
    break;
  case 3:
    return "0011";
    break;
  case 4:
    return "0100";
    break;
  case 5:
    return "0101";
    break;
  case 6:
    return "0110";
    break;
  case 7:
    return "0111";
    break;
  case 8:
    return "1000";
    break;
  case 9:
    return "1001";
    break;
  case 10:
    return "1010";
    break;
  case 11:
    return "1011";
    break;
  case 12:
    return "1100";
    break;
  case 13:
    return "1101";
    break;
  case 14:
    return "1110";
    break;
  case 15:
    return "1111";
    break;

  default:
    handleError("Error! invalid number\n", line_number);
    return NULL;
    break;
  }
}

int encode_binary_tester(int src_type, int insruction, int dst_type, int a_r_e, int src_reg, int dst_reg, int address,
char *number, int line_number) {
  if (src_type != 1 && src_type != 3 && src_type != 5 && src_type != -1){
    handleError("Error with source op variable\n", line_number);
    return 1;
  }

  /*the insruction doesn't need test. it has in the dec_to_bin func.*/

  if (dst_type != 1 && dst_type != 3 && dst_type != 5 && dst_type != -1) {
    handleError("Error with target op variable\n", line_number);
    return 1;
  }

  if (a_r_e < (-1) || a_r_e > 2) {
    handleError("Error with ARE variable\n", line_number);
    return 1;
  }

  if (((src_reg < 0 && src_reg != -1) || (src_reg > 7 && src_reg != -1)) ||
      ((dst_reg < 0 && dst_reg != -1) || (dst_reg > 7 && dst_reg != -1))){
    handleError("Error with register address\n", line_number);
    return 1;
  }

  return 0; 
}

void create_a_r_e(char *str, int a_r_e, char *bin_a_r_e, int line_number) {
  int i;
  bin_a_r_e = dec_to_bin(a_r_e, line_number);
  bin_a_r_e++;
  bin_a_r_e++; /*ignore the two first letters in the four letters bin.*/
  for (i = 0; i < 2; i++) { /*not using strncat because it adds NULL*/
    str[i + 10] = bin_a_r_e[i];
  }
}

char *bin_address(char *str, int address, int line_number) {
  char c[2];
  int j = 9;
  int num;
  num = address; 
  while (num > 0) {
    sprintf(c, "%d", (num % 2));
    str[j] = c[0];
    num = num / 2;
    j--;
    /*c[0] = NULL;*/
  }
  while (j >= 0) {
    str[j] = '0';
    j--;
  }
  return str;
}

char* data_str_or_num_to_bin(char *str, int a_r_e, char *number, int line_number) {
  int is_not_data = 0; /*value 1: the char *number isn't DATA*/
  /*prints number in the denth fist bin. digits*/
  if ((a_r_e != -1) && number != NULL) {
    char* bin_a_r_e = malloc(2*sizeof(char));
    bin_valid_number(str, number, line_number);
    create_a_r_e(str, a_r_e, bin_a_r_e, line_number);
    return str;
  }
  /*when ARE=-1 and there is a 'number' arg. it means we have data or string*/
  if ((a_r_e == -1) && number != NULL) {
    int j = 0;
    int number_length = strlen(number);
    if (number[j] != '+' && number[j] != '-' &&
        !(number[j] > 47 && number[j] < 58))
      is_not_data = 1;

    for (j = 1; j < number_length && is_not_data == 0; j++) {
      if (!(number[j] > 47 && number[j] < 58))
        is_not_data = 1;
    }
    /*if it's data - call to bin_data, else - call to bin_char*/
    if (is_not_data == 0)
      bin_data(str, number, line_number);
    else {
      bin_char(str, number, line_number);
    }
  }
  return str;
}

void bin_valid_number(char *str, char *number, int line_number){
  char c[2];
  int num;
  int j;
  int is_neg = 0; /*flag to be negative number*/
  num = atoi(number);
  /*range test: -512 to 511*/
  if (num > 511 || num < -512) {
    handleError("Error! number out of range\n", line_number);
    return;
  } else if (num <= 511 && num >= -512) {
      /*9 digits positive number (first digit is a sign bit)*/
      if (num < 0) {
        num = num * (-1);
        is_neg = 1;
      }
      j = 9;
      while (num > 0) {
        sprintf(c, "%d", (num % 2));
        str[j] = c[0];
        num = num / 2;
        j--;
      }
      while (j >= 0) {
        str[j] = '0';
        j--;
      }
      /*inverting negative number to the negative repr.*/
      if (is_neg) {
        /*case 1: when the last 'positive' number is 1*/
        if (str[9] == '1') {
          for (j = 0; j < 9; j++) { /*only to the 10'th cell because 2's com. princ.*/
            if (str[j] == '0')
              str[j] = '1';
            else 
              str[j] = '0';
          }
        }
      /*case 2: when the last number is '0'*/
        else {
          int last_zero;
          for (j = 0; j < 10; j++) {
            if (str[j] == '0')
              str[j] = '1';
            else {
              str[j] = '0';
              last_zero = j;
            }
          }
        str[last_zero] = '1';
        for (j = last_zero + 1; j < 10; j++)
          str[j] = '0';
      }
    } /*end of negaitve analize*/
  }
}

char *bin_data(char *str, char *number, int line_number) {
  char c[2];
  int num;
  int j;
  int is_neg = 0; /*flag to be negative number*/
  num = atoi(number);
  /*range test*/
  if (num > 2047 || num < -2048) {
    handleError("Error! number out of range\n", line_number);
  } else {
    /*11 digits positive number (first digit is a sign bit)*/
    if (num < 0) {
      num = num * (-1);
      is_neg = 1;
    }
    j = 11;
    while (num > 0) {
      sprintf(c, "%d", (num % 2));
      str[j] = c[0];
      num = num / 2;
      j--;
      /*c[0] = NULL;*/
    }
    while (j >= 0) {
      str[j] = '0';
      j--;
    }

    /*inverting negative number to the negative repr.*/
    if (is_neg) {
      /*case 1: when the last 'positive' number is 1*/
      if (str[11] == '1') {
        for (j = 0; j < 11;
             j++) { /*only to the 10'th cell because 2's com. princ.*/
          if (str[j] == '0')
            str[j] = '1';
          else
            str[j] = '0';
        }
      }
      /*case 2: when the last number is '0'*/
      else {
        int last_zero = 0;
        for (j = 0; j < 12; j++) {
          if (str[j] == '0')
            str[j] = '1';
          if (str[j] == '1') {
            str[j] = '0';
            last_zero = j;
          }
        }
        str[last_zero] = 1;
        for (j = last_zero + 1; j < 12; j++) {
          if (str[j] == '0')
            str[j] = '1';
          if (str[j] == '1')
            str[j] = '0';
        }
      }
    } /*end of negaitve analize*/
  }
  return str;
}

char *bin_char(char *str, char *number, int line_number) {
  char c[2];
  int num;
  int j;
  if (strlen(number) > 1)
    handleError("ERROR with givven letter\n", line_number);
  num = number[0];
  /*12 digits positive number*/
  j = 11;
  if (num == '\0') { /* for null-terminator */
    while (j >= 0) {
    str[j] = '0';
    j--;
    }
  }
  while (num > 0) {
    sprintf(c, "%d", (num % 2));
    str[j] = c[0];
    num = num / 2;
    j--; /*c[0] = NULL;*/
  }
  while (j >= 0) {
    str[j] = '0';
    j--;
  }
  return str;
}
