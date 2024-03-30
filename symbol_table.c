#include "symbol_table.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// This will act as our counter for simulated line number.
static int current_line_number = 1;

// Create new symbols
SymbolNode* create_symbol(char* name, char* type) {
    SymbolNode* node = (SymbolNode*)malloc(sizeof(SymbolNode));
    node->name = strdup(name);
    //node->type = strdup(type);
    node->type = "NULL";
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->line_number = current_line_number++; // Assign line number
    return node;
}

int getHeight(SymbolNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

// Update the height of a symbol based on the heights of its subtrees
void updateHeight(SymbolNode* node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// Check whether the node is balanced or not.
bool balanced(SymbolNode* node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    int diff = leftHeight - rightHeight;
    int d = diff < 0 ? -diff : diff;
    return d <= 1;
}

// Determine which rotation case
int checkCase(SymbolNode* node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    int balance = leftHeight - rightHeight;
    if (balance > 1) {
        if (getHeight(node->left->left) < getHeight(node->left->right))
            return 3; // Left Right Case
        else
            return 1; // Left Left Case
    }
    else if (balance < -1) {
        if (getHeight(node->right->right) < getHeight(node->right->left))
            return 4; // Right Left Case
        else
            return 2; // Right Right Case
    }
    return 0; // Balanced
}

// Right rotate the subtree
SymbolNode* rightRotate(SymbolNode* y) {
    SymbolNode* x = y->left, * T = x->right;

    x->right = y;
    y->left = T;

    updateHeight(y);
    updateHeight(x);

    return x; // New root
}

// Left rotate the subtree
SymbolNode* leftRotate(SymbolNode* x) {
    SymbolNode* y = x->right, * T = y->left;

    y->left = x;
    x->right = T;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// Rebalance the tree
void rebalance(SymbolNode** node) {
    int caseNumber = checkCase(*node);
    switch (caseNumber) {
    case 1:
        *node = rightRotate(*node);
        break;
    case 2:
        *node = leftRotate(*node);
        break;
    case 3:
        (*node)->left = leftRotate((*node)->left);
        *node = rightRotate(*node);
        break;
    case 4:
        (*node)->right = rightRotate((*node)->right);
        *node = leftRotate(*node);
        break;
    default:
        break;
    }
}

// Insert a new symbol
SymbolNode* insert_symbol(SymbolNode* root, char* name, char* type) {
    // Standard BST insertion
    if (root == NULL)
        return create_symbol(name, type);
    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        root->left = insert_symbol(root->left, name, type);
    }
    else if (cmp > 0) {
        root->right = insert_symbol(root->right, name, type);
    }
    updateHeight(root);
    rebalance(&root);

    return root;
}

// Find a symbol with the given name
SymbolNode* find_symbol(SymbolNode* root, char* name) {
    if (root == NULL)
        return NULL;
    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        return find_symbol(root->left, name);
    }
    else if (cmp > 0) {
        return find_symbol(root->right, name);
    }
    else {
        return root;
    }
}

// In-order traversal to print the symbols
void print_symbol(SymbolNode* root) {
    //if (root != NULL) {
    //    print_symbol(root->left);
    //    printf("Name: %-10s | Type: %-10s | Memory Location: %-10x | Order of insertion: %-10d\n", root->name, root->type, &root->name, root->line_number);
    //    puts("------------------------------------------------------------------------------------------");
    //    print_symbol(root->right);
    //}
    if (root != NULL) {
        print_symbol(root->left);
        printf("%s\t\t(%s)\n", root->name, root->type);
        print_symbol(root->right);
    }
}

// Function to print the entire symbol table
void print_symbol_table(SymbolNode* root) {
    printf("\nItems in Symbol Table\n");
    puts("------------------------------------");
    puts("Lexeme\t\tType");
    print_symbol(root);
    //printf("\n");
}

// Recursively destroy the tree and free the memory
void destroy_symbol_table(SymbolNode* root) {
    if (root == NULL) {
        return;
    }
    destroy_symbol_table(root->left);
    destroy_symbol_table(root->right);
    free(root->name);
    free(root);
}
