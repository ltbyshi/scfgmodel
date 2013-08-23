#include <iostream>
#include <cstdlib>

using namespace std;

#include "matrix.h"
#include "stack.h"

void Test_Matrix()
{
	Matrix1D<int> m1d(100);
	for(int i = 0; i < m1d.Size(); i ++)
		m1d(100) = i;
	Matrix1D<int> m1d2(m1d);
}

void Test_Stack()
{
	Stack<int> S;
	for(int i = 0; i < 1000; i ++)
		S.Push(i);
	for(int i = 0; i < 1000; i ++)
		S.Pop();
}

int main(int argc, char** argv)
{
	Test_Matrix();
	Test_Stack();

	return 0;
}
