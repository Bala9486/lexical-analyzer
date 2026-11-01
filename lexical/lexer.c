#include "lexer.h"

char keywords_found[100][50];
char identifiers_found[100][50];
char numbers_found[100][50];
char operators_found[100][10];
char symbols_found[100][10];

int kw_count = 0;
int id_count = 0;
int num_count = 0;
int op_count = 0;
int sym_count = 0;
int error_count =0;

char *keywords[] =
{
	"int",
	"float",
	"char",
	"if",
	"else",
	"while",
	"for",
	"return",
	"void"
};

int is_keyword(char *word)
{
	int n = sizeof(keywords) / sizeof(keywords[0]);

	for(int i = 0; i < n; i++)
	{
		if(strcmp(word, keywords[i]) == 0)
			return 1;
	}
	return 0;
}

int is_identifier(char *word)
{
	if(!(isalpha(word[0]) || word[0] == '_'))
		return 0;

	for(int i = 1; word[i] != '\0'; i++)
	{
		if(!(isalnum(word[i]) || word[i] == '_'))
			return 0;
	}

	return 1;
}

int is_number(char *word)
{
	for(int i = 0; word[i] != '\0'; i++)
	{
		if(!isdigit(word[i]))
			return 0;
	}

	return 1;
}
int is_hex_digit(char ch){
	if(isdigit(ch)){
		return 1;
	}
	if(ch>='a' && ch<='f'){
		return 1;
	}
	if(ch>='A' && ch<='F'){
		return 1;
	}
	return 0;
}
int is_hexadecimal(char *word){
	int i=2;
	if(!(word[0]=='0' && (word[1]=='x' || word[1]=='X'))){
		return 0;
	}
	if(word[2]=='\0'){
		return 0;
	}
	while(word[i]!='\0'){
		if(!is_hex_digit(word[i])){
			return 0;
		}
		i++;
	}
	return 1;
}
int is_octal(char *word){
	int i=1;
	if(word[0]!='0'){
		return 0;
	}
	if(word[1]=='\0'){
		return 1;
	}
	while(word[i]!='\0'){
		if(word[i]<'0' || word[i]>'7'){
			return 0;
		}
		i++;
	}
	return 1;
}
int is_binary(char *word){
	int i=2;
	if(!(word[0]=='0' && (word[1]=='b' || word[1]=='B'))){
		return 0;
	}
	if(word[2]=='\0'){
		return 0;
	}
	while(word[i]!='\0'){
		if(word[i]!='0' && word[i]!='1'){
			return 0;
		}
		i++;
	}
	return 1;
}


int is_operator(char ch)
{
	return (ch == '+' || ch == '-' ||
			ch == '*' || ch == '/' ||
			ch == '=' || ch == '!' ||
			ch == '%' || ch == '^' ||
			ch == '&' || ch == '~' ||
			ch == '<' || ch == '>' ||
			ch == '|');
}
int is_symbol(char ch)
{
	return (ch == ';' || ch == ',' ||
			ch == '(' || ch == ')' ||
			ch == '{' || ch == '}' ||
			ch == '.' || ch == '@' ||
			ch == '#' || ch == '$');
}
int is_string(char *word)
{
	int len = strlen(word);

	if(len >= 2 && word[0] == '"' && word[len - 1] == '"')
		return 1;

	return 0;
}


void print_token(char *lexeme, int token_type)
{
	switch(token_type)
	{
		case KEYWORD:
			printf("Keyword    : %s\n", lexeme);
			strcpy(keywords_found[kw_count++], lexeme);
			break;

		case IDENTIFIER:
			printf("Identifier : %s\n", lexeme);
			strcpy(identifiers_found[id_count++], lexeme);
			break;

		case NUMBER:
			printf("Literal    : %s\n", lexeme);
			strcpy(numbers_found[num_count++], lexeme);
			break;

		case OPERATOR:
			printf("Operator   : %s\n", lexeme);
			strcpy(operators_found[op_count++], lexeme);
			break;

		case SYMBOL:
			printf("Operator   : %s\n", lexeme);
			strcpy(symbols_found[sym_count++], lexeme);
			break;
	}
}
void analyze_file(FILE *fp)
{
	int ch;
	char token[100];
	int i;

	int round_bracket=0;
	int curly_bracket=0;
	int double_quote=0;
	int single_quote=0;

	while((ch = fgetc(fp)) != EOF)
	{
		if(ch =='"'){
			if(single_quote==0){
				double_quote=!double_quote;
			}
			continue;
		}
		if(ch =='\''){
			if(double_quote==0){
				single_quote =!single_quote;
			}
			continue;
		}
		if(double_quote){
			continue;
		}
		if(single_quote){
			continue;
		}

		if(isalpha(ch) || ch == '_')
		{
			i = 0;

			while(isalnum(ch) || ch == '_')
			{
				token[i++] = ch;
				ch = fgetc(fp);
			}

			token[i] = '\0';

			if(ch != EOF)
				ungetc(ch, fp);

			if(is_keyword(token))
				print_token(token, KEYWORD);
			else if(is_identifier(token))
				print_token(token, IDENTIFIER);
		}

		else if(isdigit(ch))
		{
			i = 0;

			while(isalnum(ch) || ch =='_')
			{
				token[i++] = ch;
				ch = fgetc(fp);
			}

			token[i] = '\0';

			if(ch != EOF)
				ungetc(ch, fp);

			if(isalpha(token[0])==0 && (strchr(token,'_')!=NULL || isalpha(token[1]))){
				if(strncmp(token,"0x",2)==0 ||strncmp(token,"0X",2)==0){
					if(is_hexadecimal(token)){
						print_token(token,NUMBER);
					}else{
						printf("Error: Invalid hexadecimal value: %s\n",token);
						error_count++;
					}
				}
				else if(strncmp(token,"0b",2)==0 || strncmp(token,"0B",2)==0){
					if(is_binary(token)){
						print_token(token,NUMBER);
					}else{
						printf("Error: Invalid binary value:%s\n",token);
						error_count++;
					}
				}else{
					printf("Error:Invalid identifier: %s\n",token);
					error_count++;
				}
			}

			else if(token[0]=='0'&&(token[1]=='x'||token[1]=='X')){
				if(is_hexadecimal(token)){
					print_token(token,NUMBER);
				}else{
					printf("Error: Invalid hexadecimal value:%s\n",token);
					error_count++;
				}
			}
			else if(token[0]=='0' && (token[1]=='b' || token[1]=='B')){
				if(is_binary(token)){
					print_token(token,NUMBER);
				}else{
					printf("Error: Invalid binary value:%s\n",token);
					error_count++;
				}
			}
			else if(token[0]=='0' && token[1]!='\0'){
				if(is_octal(token)){
					print_token(token,NUMBER);
				}else{
					printf("Error: Invalid octal value: %s\n",token);
					error_count++;
				}
			}
			else if(is_number(token)){
				print_token(token,NUMBER);
			}
			else{
				printf("Error: Invalid number: %s\n",token);
				error_count++;
			}

		}
		else if(is_operator(ch))
		{
			token[0] = ch;
			token[1] = '\0';

			print_token(token, OPERATOR);
		}

		else if(is_symbol(ch))
		{
			token[0] = ch;
			token[1] = '\0';

			if(ch == '(')
				round_bracket++;
			else if(ch == ')')
			{
				if(round_bracket > 0)
					round_bracket--;
				else
				{
					printf("Error: ')' without opening '('\n");
					error_count++;
				}
			}
			else if(ch == '{')
				curly_bracket++;
			else if(ch == '}')
			{
				if(curly_bracket > 0)
					curly_bracket--;
				else
				{
					printf("Error: '}' without opening '{'\n");
					error_count++;
				}
			}

			print_token(token, OPERATOR);
}
		}	
			if(round_bracket > 0)
		{
			printf("Error: '(' was opened but not closed\n");
			error_count++;
		}

		if(curly_bracket > 0)
		{
			printf("Error: '{' was opened but not closed\n");
			error_count++;
		}

		if(double_quote)
		{
			printf("Error: Double quote was opened but not closed\n");
			error_count++;
		}

		if(single_quote)
		{
			printf("Error: Single quote was opened but not closed\n");
			error_count++;
		}
		printf("----------------------------------------\n");
		printf("         LEXICAL ANALYZER OUTPUT\n");
		printf("----------------------------------------\n");

}	

