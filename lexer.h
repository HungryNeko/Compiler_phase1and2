#include "symbol_table.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
extern int tran_table[5][6];
extern int line_num[1024];
extern int column_num[1024];
extern char str_arr[1024][100];
bool keywords(char* str);
bool isBool(char* str);
bool isLiteral(char* str);
bool isIdentifier(char* str);
bool isSymbol(char* str);
char* next_token();
void splitLanguage(char buffer[], char str_arr[][100], int line_num[], int* len);