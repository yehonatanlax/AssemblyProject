#ifndef UTILS_H
#define UTILS_H

#define LINE_LENGTH 80
#define SUFFIX 4 /*the suffix of the files we create.*/


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




#endif

