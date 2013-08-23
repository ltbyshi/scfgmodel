#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <algorithm>

template <class T>
class Sequence
{
public:
	Sequence(int capacity = 100)
	{
		this->capacity = capacity;
		size = 0;
		data = new T[capacity];
	}

	~Sequence()
	{
		if(data)
			delete data;
	}

	void Append(const T & e)
	{
		if(size >= capacity)
			Expand();
		data[size] = e;
		size ++;
	}

	int Size() const
	{
		return size;
	}

	T& operator()(int i)
	{
		return data[i];
	}

	T& operator[](int i)
	{
		return data[i];
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
