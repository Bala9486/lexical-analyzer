#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*Token Type*/
#define KEYWORD     1
#define IDENTIFIER  2
#define NUMBER      3
#define OPERATOR    4 
#define SYMBOL      5
#define STRING      6
#define UNKNOWN     7


/*Function Declaration*/
int is_keyword(char *word);
int is_identifier(char *word);
int is_number(char *word);
int is_hexadecimal(char *word);
int is_octal(char *word);
int is_binary(char *word);
int is_operator(char ch);
int is_symbol(char ch);
int is_string(char *word);

void analyze_file(FILE *fp);
void print_token(char *lexeme, int token_type);

#endif





