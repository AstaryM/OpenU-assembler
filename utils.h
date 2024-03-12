/*general util functions*/

#ifndef UTILS
#define UTILS

#include "definitions.h"

/*just a malloc with verification*/
void *verified_malloc(int size);

/*makes dest into a substring from "start" index to "end" index of str*/
void substr(char *dest, char *str, int start, int end);

/*breaks a line into an array of words*/
int break_line(char *list[], char *str);

/*delete all the legal commas in a line object and returns whether the commas placement is valid. the new value of num_of_words is the new number of words after deleting the commas*/
bool delete_commas(line_ob line, int *num_of_words);

/*returns the nth word of a string*/
void get_nth_word(char *dest, char *line, int n);

/*returns whether a substring of a string with length strlen-n is a reserved words*/
bool is_reserved(char *str, int n);

/*returns whether a substring of a string with length strlen-n is a legal label name*/
bool is_legal_label(char *str, int n);

/*returns whether a string consists of numeric chars*/
bool is_str_num(char *str);


#endif
