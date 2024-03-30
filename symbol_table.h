// Define the stucture of Symbolnode in the AVL tree
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct SymbolNode {
    char* name;
    char* type;
    int height;
    int line_number; // Simulated insertion sequence for line number.
    struct SymbolNode* left;
    struct SymbolNode* right;
} SymbolNode;

#endif
int getHeight(SymbolNode* node);
SymbolNode* insert_symbol(SymbolNode* root, char* name, char* type);
SymbolNode* find_symbol(SymbolNode* root, char* name);
void print_symbol(SymbolNode* root);
void destroy_symbol_table(SymbolNode* root);
void print_symbol_table(SymbolNode* root);
