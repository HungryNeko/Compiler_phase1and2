#include "lexer.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
int tran_table[5][6] = {
  //Letter Number others   -      >    ' '/'\n'
	{ 1,     2,     3,     4,     3,     0  }, // q0
	{ 1,     1,     0,     0,     0,     0  }, // q1
	{ 0,     2,     0,     0,     0,     0  }, // q2
	{ 0,     0,     0,     0,     0,     0  }, // q3
	{ 0,     0,     0,     0,     3,     0  }, // q4
};
int token_num = -1, line_num[1024], column_num[1024];
char str_arr[1024][100];
bool keywords(char* str) {
	return (strcmp(str, "fun") == 0 || strcmp(str, "nat") == 0 || strcmp(str, "->") == 0 || strcmp(str, "bool") == 0);
}
bool isBool(char* str) {
	return (strcmp(str, "T") == 0 || strcmp(str, "F") == 0);
}
bool isLiteral(char* str) {
	while (*str != '\0') {
		if (!isdigit(*str))
			return false;
		str++;
	}
	return true;
}
bool isIdentifier(char* str) {
	if (str == NULL || *str == '\0') {
		return false;
	}
	if (!isalpha(*str)) {
		return false;
	}
	while (*(++str) != '\0') {
		if (!isalnum(*str)) {
			return false;
		}
	}
	return true;
}
bool isSymbol(char* str) {
	return (strcmp(str, "?") == 0 || strcmp(str, "+") == 0 || strcmp(str, ":") == 0 || strcmp(str, ";") == 0 || strcmp(str, "=") == 0 || strcmp(str, "<") == 0 || strcmp(str, "&") == 0 || strcmp(str, "(") == 0 || strcmp(str, ")") == 0);
}
char* next_token() {
	++token_num;
	if (keywords(str_arr[token_num]) || isSymbol(str_arr[token_num]))
		return str_arr[token_num];
	else if (isBool(str_arr[token_num]))
		return "literal(Boolean)";
	else if (isLiteral(str_arr[token_num]))
		return "literal(natural)";
	else if (isIdentifier(str_arr[token_num]))
		return "identifier";
	else
		return "Lexical error";
}
void DFA(int* state, char Putin) {
	if (isalpha(Putin)) {
		*state = tran_table[*state][0];
	}
	else if (isdigit(Putin)) {
		*state = tran_table[*state][1];
	}
	else if (Putin == '-') {
		*state = tran_table[*state][3];
	}
	else if (Putin == '>')
		*state = tran_table[*state][4];
	else if (Putin == ' ' || Putin == '\n' || Putin == '\t')
		*state = tran_table[*state][5];
	else {
		*state = tran_table[*state][2];
	}
}
void splitLanguage(char buffer[], char str_arr[][100], int line_num[], int* len) {
	int state = 0, buffer_size = 0, line = 1;
	column_num[0] = 1;
	for (int i = 0, column = 1; buffer[i] != '\0'; i++, column++) {
		if (buffer[i] == '[') {
			while (buffer[i] != ']') {
				i++;
			}
			continue;
		}
		DFA(&state, buffer[i]);
		if (state == 0 && buffer_size != 0) {
			buffer_size = 0;
			//column_num[++(*len)] = column;
			++(*len);
			DFA(&state, buffer[i]);
		}
		if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t'){
			if(buffer[i] == '\n')
				line++, column = 0;
		}
		else{
			if(buffer_size == 0)
				column_num[(*len)] = column;
			str_arr[*len][buffer_size++] = buffer[i];
		}
		line_num[*len] = line;
	}
	if (buffer_size == 0) (*len)--;
}