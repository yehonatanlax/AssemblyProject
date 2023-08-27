#ifndef _ENCODE_H_
#define _ENCODE_H_
#define BIN_LINE 13


/*main function of encoding the given data to binary lines*/
char *encode_binary(int src_type, int insruction, int dst_type, int a_r_e, int src_reg, int dst_reg, int address, char* number, int line_number);

/*convert the decimal instruction/operand/ARE to binary*/
char *dec_to_bin(int num, int line_number);

/*tester for the given argum. in 'encode_binary'*/                      
int encode_binary_tester(int src_type, int insruction, int dst_type, int a_r_e, int src_reg, int dst_reg, int address, char* number, int line_number);

/*create the binary representation of the ARE*/                      
void create_a_r_e(char *str, int a_r_e, char *bin_a_r_e, int line_number);

/*convert decimal address to the  tenth first digits in the binary line*/
char *bin_address(char *str, int address, int line_number);

/*convert data or string to binary*/
char *data_str_or_num_to_bin(char *str, int a_r_e, char *number, int line_number);

/*encode a valid number into 10'th digit + ARE*/
void bin_valid_number(char *str, char *number, int line_number);

/*convert data in the 2's comp. rep. (range: -2048 to 2047)*/
char *bin_data(char *str, char *number, int line_number);

/*convert a specific char from a string to binary rep.*/
char *bin_char(char *str, char *number, int line_number);

/*converting binary functions*/
char *bin_to_b64_sign(char *, char *);

int bin_to_dec(char *);

char dec_to_b64(int);

char *split_bin_to_first_part(char *bin_num);

char *split_bin_to_second_part(char *bin_num);

#endif