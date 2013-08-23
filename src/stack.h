#ifndef __STACK_H__
#define __STACK_H__

struct Stack{
	int capacity;
	int size;
	int frameSize;
	void* data;
};

Stack* CreateStack(int frameSize);
void FreeStack(Stack* S);
void PushStack(Stack* S, void* elem);
void* PopStack(Stack* S);
int IsStackEmpty(Stack* S);
void* StackTop(Stack* S);

#endif
