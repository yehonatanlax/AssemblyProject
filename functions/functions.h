#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 31
#define BIN_LENGTH 12 
#define HALF_BIN_LEN 6
#define B_64_LEN 2


void remove_spaces_before(char *str);

char * get_first_word(char * str);

char* checkFirstWord(char * word);

int ignore(char * line);

int is_line_empty_or_comment(char* line);

int can_word_be_valid_label(char* word, int line_number);

int is_word_a_valid_label_declaration(char* word, int line_number);

char* is_directive(char* line);

int is_instruction(char* line);

void handleError(char* err, int line_number);

char* findNextWord(int size, char* line);

int instruction_params_num(char* instruction);

void handle_instruction(char* instruction, char* line, char* label, int line_number);

void handle_directive(char* instruction, char* line, char* label, int line_number);

int type_of_operand(char* operand, int line_number);

void move_line_ptr_to_next_word(char* word, char* line);

/* instruction: src_type , dst_type , instruction = num of instruction , ARE, src_reg && dst_reg = -1 . address = NULL */
/* register: src_type = will be 1/3/5 or -1 if it's a dst register. dst_type = same. instruction = will be -1. ARE . src_reg && dst_reg = number of register. will be -1 if only one register. address = NULL */
/* label: address = address of label , ARE. all the rest = -1 */
/* number: number , ARE. all the rest = -1 */
char* encode_binary(int src_type, int insruction, int dst_type, int ARE, int src_reg, int dst_reg, int address, char* number, int line_number);

int remove_comma(char* line, int line_number);

int is_number(char* s);

int check_if_valid_register(char* reg, int line_number);

void add_line_to_ic(char** encoded_operands, char* instruction, char* label, int num_of_lines, int operands_num, int line_number, int src_type, int dst_type, char* op_to_bl);

void handle_command(char * line, char* label, int line_number);

int get_register_number(char* reg);

void clear_error_flag();

#endif