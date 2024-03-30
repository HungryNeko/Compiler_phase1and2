#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
SymbolNode* root = NULL;
int line_num[1024], column_num[1024], len, token_num, error_type, token_type, returnValue;
char str_arr[1024][100];
char* token;

int main(int argc, char* argv[]) {
    char prefix[256], newFileName[256];
    char* fileName = argv[1];
	FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    sscanf(fileName, "%[^.]", prefix);
    snprintf(newFileName, sizeof(newFileName), "%s_out.txt", prefix);
    size_t buffer_size = 1024;
    char* buffer = (char*)malloc(buffer_size * sizeof(char));
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }
    size_t length = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        buffer[length++] = (char)ch;
        if (length >= buffer_size) {
            buffer_size *= 2;
            buffer = (char*)realloc(buffer, buffer_size * sizeof(char));
            if (buffer == NULL) {
                perror("Memory reallocation error");
                fclose(file);
                return 1;
            }
        }
    }
    buffer[length] = '\0';
    splitLanguage(buffer, str_arr, line_num, &len);
    puts("\n------Expected Outcome------\nLexeme\t\t\tToken\n----------------------------");
    for (int i = 0; i <= len; i++) {
        token = next_token(&token_num);
        if (strcmp(token, "Lexical error") == 0) {
            printf("\"%s\"\t\t\t%s\n", str_arr[token_num], token);
            printf("Error: Invalid Token \'%s\' found at line: %d, column %d\nLexical Analysis Halt since an invalid token is detected!\n", str_arr[token_num], line_num[token_num], column_num[token_num]);
            break;
        }
        if (strcmp(token, "identifier") == 0)
            root = insert_symbol(root, str_arr[token_num], token);
        printf("\"%s\"\t\t\t%s\n", str_arr[token_num], token);
        storetable(str_arr[token_num], token);
    }
    puts("----------------------------");
    print_symbol_table(root);
    //passTable(newFileName,line_num);//change!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    returnValue = passTable(newFileName);
    if (returnValue == -1) {
        printf("\nParsing Accomplished! No syntax error!\n");
    }
    else {
        printf("\nSyntax error on line %d!\n", line_num[returnValue]);
    }
    free(buffer);
    fclose(file);
    return 0;
}
