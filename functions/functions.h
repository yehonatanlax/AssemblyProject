#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 31
#define BIN_LENGTH 12 
#define HALF_BIN_LEN 6
#define B_64_LEN 2


/*converting binary functions*/
char bin_to_b64_sign(char *);

int bin_to_dec(char *);

char dec_to_b64(int);

char *split_bin(char *);

char *analize_bin(char *);

char *write_binary_line(*temp_binary);

char *create_file_name(char *original, int format);

void removeSpacesBefore(char *str);

char * getFirstWord(char * str);

char* checkFirstWord(char * word);

int ignore(char * line);

int isLineEmptyOrComment(char* line);

int can_word_be_valid_label(char* word, int line_number);

int is_word_a_valid_label_declaration(char* word, int line_number);

char* isDirective(char* line);

char* isInstruction(char* line);

void handleError(char* err, int line_number);

char* findNextWord(int size, char* line);

int instruction_params_num(char* instruction);

int count_word(char* line);

void handle_instruction(char* instruction, char* line, char* label, int line_number);

void handle_directive(char* instruction, char* line, char* label, int line_number);

int type_of_operand(char* operand, int line_number);

void move_line_ptr_to_next_word(char* word, char* line);

char* encode_operand(char* operand);

char* encode_two_regs(char* reg_src, char* reg_dst);

int remove_comma(char* line, int line_number);

int is_number(char* s);

int check_if_valid_register(char* reg, int line_number);

void add_line_to_ic(char** encoded_operands, char* instruction, char* label, int num_of_lines, int operands_num, int line_number, int src_type, int dst_type);

void handle_command(char * line, char* label, int line_number);

#endif