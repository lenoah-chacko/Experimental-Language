#include<stdio.h>
#include<stdlib.h>
/*
#define CONNECTOR 0
#define READING 1
#define WRITING 2
#define PLUS 3
#define MUL 4
#define SUB 5
#define DIV 6
#define CONST 7
#define IDFR 8
#define EQ 9
#define IF 10
#define IFELSE 11
#define LT 12
#define GT 13
#define EQUCHECK 14
#define LE 15
#define GE 16
#define NE 17
#define While 18
#define FUN 19
#define RET 20
*/
#define INTGR 1
#define BOOLEAN 2

//read/write/connector/+/*
typedef struct tnode { 
	int val;	// value of a number for NUM nodes.
	int type;	//type of variable
	char* varname;	//name of a variable for ID nodes
	struct node *symbol;  
	int nodetype;  // information about non-leaf nodes - read/write/connector/+/* etc.  
	struct tnode *left,*right;	//left and right branches   
}tnode;
	
/*Make a leaf tnode and set the value of val field*/
struct tnode* makeLeafNode(int n, int type);
struct tnode* makeLeafNodeID( int type,char* name);
	
/*Make a tnode with opertor, left and right branches set*/
struct tnode* makeOperatorNode(int c,struct tnode *l,struct tnode *r);
	
/*To evaluate an expression tree*/
int evaluate(FILE *F,struct tnode *t);

