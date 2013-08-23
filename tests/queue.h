#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <cassert>

template <class T>
class Queue
{
public:
	Queue(int capacity = 100)
	{
		this->capacity = capacity;
		size = 0;
		data = new T[capacity];
		head = 0;
		tail = 0;
	}

	~Queue()
	{
		delete data;
	}
	
	void Push(const T& e)
	{
		if(size >= capacity)
			Expand();
		data[tail] = e;
		tail = (tail + 1)%capacity;
		size ++;
	}
	
	T Pop()
	{
		assert(size > 0);

		T e = data[head];
		head = (head + 1)%capacity;
		size --;

		return e;
	}

	T Top() const
	{
		assert(size > 0);

		return data[head];
	}

	bool Empty() const
	{
		return (size <= 0);
	}

private:
	void Expand()
	{
		T* oldData = data;
		data = new T[capacity*2];
		for(int i = 0; i < size; i ++)
			data[i] = oldData[(head + i)%capacity];
		delete oldData;
		capacity *= 2;
		head = 0;
		tail = size;
	}

private:
	int head;
	int tail;
	int capacity;
	int size;
	T* data;
};

#endif
