/*
 * parse_tree.h
 *
 *  Created on: 2023年12月14日
 *      Author: Inkling
 */

#ifndef PARSE_TREE_H_
#define PARSE_TREE_H_



#endif /* PARSE_TREE_H_ */
struct Node {
    int data;
    struct Node* p[5];
};
struct Node* newNode(int data,struct Node* nodearry[],int num);
void printNodeTree(struct Node* root);
