#ifndef STACK_H
#define STACK_H
typedef struct{
char* name;
int line_num;
} StructState;

typedef struct{
    StructState stk[STACKSIZE];
    int top;
}Stack;

#endif
