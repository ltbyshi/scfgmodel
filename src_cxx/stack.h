#ifndef __STACK_H__
#define __STACK_H__

#include <cassert>
#include <algorithm>

#define DEFAULT_STACK_CAPACITY 100

template <class T>
class Stack
{
public:
	Stack(int capacity = DEFAULT_STACK_CAPACITY)
	{
		this->capacity = capacity;
		data = new T[capacity];
		size = 0;
	}

	~Stack()
	{
		delete data;
	}

	void Push(const T& e)
	{
		if(size >= capacity)
			Expand();
		data[size] = e;
		size ++;
	}

	T Pop()
	{
		assert(size > 0);

		size --;
		return data[size];
	}

	T Top() const
	{
		assert(size > 0);

		return data[size - 1];
	}

	bool Empty() const
	{
		return (size <= 0);
	}
private:
	void Expand()
	{
		T* oldData = data;
		capacity *= 2;
		data = new T[capacity];
		std::copy(oldData, oldData + size, data);
		delete oldData;
	}
private:
	int capacity;
	int size;
	T* data;
};

#endif
