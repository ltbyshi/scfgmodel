#ifndef __VECTOR_H__
#define __VECTOR_H__

template <class T>
class Vector
{
public:
	Vector(int size = 100)
	{
		this->size = size;
		data = new T[size];
	}

	~Vector()
	{
		if(data)
			delete data;
	}

	void Resize(int size, bool reserve = true)
	{
		if(reserve)
		{
			T* oldData = data;
			data = new T[size];
			if(size > this->size)
				std::copy(oldData, oldData + this->size, data);
			else
				std::copy(oldData, oldData + size, data);
			delete oldData;
			this->size = size;
		}
		else
		{
			if(data)
				delete data;
			data = new T[size];
			this->size = size;
		}
	}

	int Size() const
	{
		return size;
	}

	T& operator[](int i) const
	{
		return data[i];
	}

	T& operator[](int i)
	{
		return data[i];
	}
private:
	int size;
	T* data;
};

#endif
