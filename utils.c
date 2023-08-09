#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#define BIN_LENGTH 12 
#define HALF_BIN_LEN 6
#define B_64_LEN 2

/*Splits the string of the bin-num and convert to base 64 string*/
/*TODO check is it legaL TO RETURN an inner string*/
char *analize_bin(char *bin_num){
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

   /*printf("my firast is: %s\n", first_bin);
    printf("my sec is: %s\n", second_bin);
*/

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

char dec_to_b64(int num){
  char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  return base64[num];
}