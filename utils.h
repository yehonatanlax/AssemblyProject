#ifndef UTILS_H
#define UTILS_H

#define LINE_LENGTH 80
#define SUFFIX 4 /*the suffix of the files we create.*/

/* Enum of commands ordered by their opcode */
enum commands {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, UNKNOWN_COMMAND};
/*using the enum commands and converting the command to its binary rep.*/
 char *commands_binary[] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", 
"1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};


/* Types of files that indicate what is the desirable file extension */
enum filetypes {AM};

/*converting binary functions*/
char bin_to_b64_sign(char *);
int arr_len(char *);
int bin_to_dec(char *);
char dec_to_b64(int);
char *split_bin(char *);
char *analize_bin(char *);

/*tools for analize the lines*/
char* isInstruction(char *);
int isValidLabel(char *);
int isFirstWordLabel(char *);
char *create_file_name(char *original, int format);
char* space_jump(char *);

/*fubctions for second pass*/
void sec_pass(FILE *, char *);
char *parse_word(char *);
char *write_binary_line(*temp_binary);



#endif

