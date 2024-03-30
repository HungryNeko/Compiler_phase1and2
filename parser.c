#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "parse_tree.h"
#define State int
#define Symbol int
#define STACK_SIZE 10000
typedef struct {
	char *str, *token;
} QueueElement;

typedef struct {
	QueueElement* elements;
	int size, capacity;
} Queue;
//int currnodes=0;
struct Node* nodes[STACK_SIZE];
int stack_top = -1, top = -1, step = 0, allElementsProcessed = 0, pt[38][26][2], gramRule[19][6];
char store[1000] = "\n", temp[1000] = "";
State state_stack[STACK_SIZE];
Symbol symbol_stack[STACK_SIZE];
struct Node* treeNode[STACK_SIZE];
Queue queue = { NULL, 0, 0 };

void add(struct Node* node) {
	if (top >= STACK_SIZE - 1) {
		printf("Stack Overflow\n");
		return;
	}
	treeNode[++top] = node;
}

struct Node* popStack() {
	if (top < 0) {
		printf("Stack Underflow\n");
		return NULL;
	}
	return treeNode[top--];
}

void push(State state, Symbol symbol) {
	if (stack_top < STACK_SIZE - 1) {
		stack_top++;
		state_stack[stack_top] = state;
		symbol_stack[stack_top] = symbol;
	} else {
		printf("Stack Overflow\n");
	}
}

void pop(int count) {
	stack_top -= count;
	if (stack_top < -1) {
		stack_top = -1;
		printf("Stack Underflow\n");
	}
}
char* printrule(int i) {
	switch (i) {
	case 0:
		return "S' -> S";
	case 1:
		return "S  -> F ; S";
	case 2:
		return "S  -> C";
	case 3:
		return "F  -> fun id A -> C";
	case 4:
		return "A  -> T id A";
	case 5:
		return "A  -> T id";
	case 6:
		return "T  -> nat";
	case 7:
		return "T  -> bool";
	case 8:
		return "C  -> B ? E : C";
	case 9:
		return "C  -> E";
	case 10:
		return "E  -> ( id I )";
	case 11:
		return "I  -> E I";
	case 12:
		return "I  -> E";
	case 13:
		return "E  -> lit + E";
	case 14:
		return "E  -> lit";
	case 15:
		return "B  -> E R E & B";
	case 16:
		return "B  -> E R E";
	case 17:
		return "R  -> <";
	case 18:
		return "R  -> =";
	}
	return "";
}
int maptable(char *str) {
	if (strcmp(str, ";") == 0)
		return 0;
	else if (strcmp(str, "fun") == 0)
		return 1;
	else if (strcmp(str, "identifier") == 0)
		return 2;  //id
	else if (strcmp(str, "->") == 0)
		return 3;
	else if (strcmp(str, "nat") == 0)
		return 4;  //nat
	else if (strcmp(str, "bool") == 0)
		return 5;  //bool
	else if (strcmp(str, "?") == 0)
		return 6;
	else if (strcmp(str, ":") == 0)
		return 7;
	else if (strcmp(str, "(") == 0)
		return 8;
	else if (strcmp(str, ")") == 0)
		return 9;
	else if (strcmp("literal(Boolean)", str) == 0 || strcmp(str, "literal(natural)") == 0)
		return 10;  //lit
	else if (strcmp(str, "+") == 0)
		return 11;
	else if (strcmp(str, "&") == 0)
		return 12;
	else if (strcmp(str, "<") == 0)
		return 13;
	else if (strcmp(str, "=") == 0)
		return 14;
	else if (strcmp(str, "$") == 0)
		return 15;
	return -1;
}
const char* mapIndexToSymbol(int index) {
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

void ensureCapacity(Queue *q) {
	if (q->size >= q->capacity) {
		q->capacity = q->capacity == 0 ? 4 : q->capacity << 1;
		q->elements = realloc(q->elements, q->capacity * sizeof(QueueElement));
	}
}

void enqueue(Queue *q, char *str, char *token) {
	ensureCapacity(q);
	q->elements[q->size].str = strdup(str);
	q->elements[q->size].token = strdup(token);
	q->size++;
}

void storetable(char *str, char *token) {
	//printf("\"%s\"\t\t\t%s\n", str, token);
	enqueue(&queue, str, token);
}

int reduce(int ruleNumber) {
	// 获取规约规则的信息
	int popCount = 1;
	while (popCount < 6 && gramRule[ruleNumber][popCount] != -1) {
		popCount++;
	}

	// 弹出相应数量的状态和符号
	pop(popCount - 1); // 最后一个是规约后的非终结符号，所以不需要弹出

	for (int i = 0; i < popCount - 1; i++) {
		nodes[i] = popStack();
	}

	// 获取规约后的非终结符号索引
	int nonTerminalIndex = gramRule[ruleNumber][0];

	// 根据非终结符号和当前栈顶状态查找GOTO表来获得新状态
	State current_state = state_stack[stack_top];
	State next_state = pt[current_state][nonTerminalIndex][1]; // 假设goto部分在pt表的相应位置

	// 将新的状态和非终结符号推入栈中
	push(next_state, nonTerminalIndex);
	//struct Node* new = newNode(nonTerminalIndex, currnodes, nodes, popCount - 1);
	//struct Node* new=newNode(nonTerminalIndex,currnodes,nodes,popCount-1);//创建新node，根据index存pointer
	//currnodes=currnodes-popCount+1;
	add(newNode(nonTerminalIndex, nodes, popCount - 1));
	/*stack_top = -1;*/
	//printf("reduce pt[%d][%d][%d]--%d\n", current_state,nonTerminalIndex,1,pt[current_state][nonTerminalIndex][1]);
	//memset(nodes, NULL, sizeof(nodes));
	return 0;
}

int passTable(char* outputFile) {
	setbuf(stdout, NULL);
	//puts("");
	push(0, -1);  // 初始状态和符号
	FILE* fp = fopen(outputFile, "w");
	for (int i = 0; i <= queue.size || !allElementsProcessed;) {
		int column, action, value;
		QueueElement element;
		if (i < queue.size) {
			element = queue.elements[i];
			column = maptable(element.token);  // 将token映射到分析表的列索引
		} else {
			// 所有元素都已处理完毕，但可能仍需执行规约操作
			allElementsProcessed = 1;
			column = 15; // 设置为无效值
		}
		action = pt[state_stack[stack_top]][column][0];  // 获取动作
		value = pt[state_stack[stack_top]][column][1];  // 获取规约规则编号或新状态

		sprintf(temp, "Step %d:<0, ", step++);
		for (int j = 1; j <= stack_top; j++) {
			sprintf(temp, "%s(%s %d), ", temp, mapIndexToSymbol(symbol_stack[j]), state_stack[j]);
		}
		fprintf(fp, "%-170s%s", temp, store);
		temp[0] = '\0';
		//printf("%d",currnodes);
		if (action == 0) { // Shift
			sprintf(store, "shift %d\n", value);
			push(value, column); // 移入新状态和符号
			i = i < queue.size ? i + 1 : i; // 仅在队列中还有元素时递增i
			//nodes[currnodes]=newNode(column,0,nodes,0);//新建一个node，存入指针表,指针都为NULL
			add(newNode(column, NULL, 0));
			//currnodes++;

		} else if (action == 1) { // Reduce
			sprintf(store, "reduce %s\n", printrule(value));
			if (reduce(value)) {
				sprintf(temp, "Step %d:<0, ", step++);
				for (int j = 1; j <= stack_top; j++)
					sprintf(temp, "%s(%s %d), ", temp, mapIndexToSymbol(symbol_stack[j]), state_stack[j]);
				sprintf(temp, "%s$", temp);
				sprintf(store, "syntax error\n");
				//printf( "syntax error on line %d!\n",line_num[i]);
				fprintf(fp, "%-170s%s", temp, store);
				return i;
			}
		} else if (action == 2) { // Accept
			sprintf(temp, "Step %d:<0, ", step++);
			for (int j = 1; j <= stack_top; j++)
				sprintf(temp, "%s(%s %d), $", temp, mapIndexToSymbol(symbol_stack[j]), state_stack[j]);
			sprintf(store, "accept\n");
			//printf("Parsing Accomplished! No syntax error!");
			//printf("\n");
			//printNodeTree(popStack());
			//break;
			fprintf(fp, "%-170s%s", temp, store);
			return -1;
		} 
		else {
			sprintf(temp, "Step %d:<0, ", step++);
			for (int j = 1; j <= stack_top; j++)
				sprintf(temp, "%s(%s %d), ", temp, mapIndexToSymbol(symbol_stack[j]), state_stack[j]);
			sprintf(temp, "%s$", temp);
			sprintf(store, "syntax error\n");
			//printf( "syntax error on line %d!\n",line_num[i]);
			//break;
			fprintf(fp, "%-170s%s", temp, store);
			return i;
		}
	}
	//fprintf(fp, "%-170s%s", temp, store);
}
