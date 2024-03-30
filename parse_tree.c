/*
 * parse_tree.c
 *
 *  Created on: 2023年12月14日
 *      Author: Inkling
 */
#include <stdio.h>
#include <stdlib.h>
#include "parse_tree.h"

struct Node* newNode(int data,struct Node* nodearry[],int num){
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	for(int i=0;i<5;i++){
		newNode->p[i]=NULL;
	}
	for(int i=0; i<num; i++){
		newNode->p[i]=nodearry[i];
	}
	return newNode;
}
const char* maping(int index) {
	switch (index) {
	case 0:
		return ";";
	case 1:
		return "fun";
	case 2:
		return "id";
	case 3:
		return "->";
	case 4:
		return "nat";
	case 5:
		return "bool";
	case 6:
		return "?";
	case 7:
		return ":";
	case 8:
		return "(";
	case 9:
		return ")";
	case 10:
		return "lit";
	case 11:
		return "+";
	case 12:
		return "&";
	case 13:
		return "<";
	case 14:
		return "=";
	case 15:
		return "$";
	case 16:
		return "primeS";
	case 17:
		return "S";
	case 18:
		return "F";
	case 19:
		return "A";
	case 20:
		return "T";
	case 21:
		return "C";
	case 22:
		return "E";
	case 23:
		return "I";
	case 24:
		return "B";
	case 25:
		return "R";
	default:
		return NULL;
	}
}
void printNodeTree(struct Node* root) {
	// 创建一个队列，用于存储每一层的节点
	struct Node** queue = malloc(1000 * sizeof(struct Node*));
	int front = 0;
	int rear = 0;

	// 将根节点加入队列
	queue[rear++] = root;
	// 添加一个NULL作为层级标记
	queue[rear++] = NULL;

	while (front != rear) {
		// 取出队列中的第一个节点
		struct Node* node = queue[front++];

		// 如果节点是NULL，表示这一层的节点已经打印完毕
		if (node == NULL) {
			puts("");
			// 如果队列中还有节点，添加一个NULL作为下一层的标记
			if (front != rear) {
				queue[rear++] = NULL;
			}
		}
		else {
			// 打印节点数据
			printf("%s ", maping(node->data));

			// 将该节点的所有子节点加入队列
			for (int i = 0; i < 5; i++) {
				if (node->p[i] != NULL) {
					queue[rear++] = node->p[i];
				}
			}
		}
	}

	// 释放队列
	free(queue);
}

