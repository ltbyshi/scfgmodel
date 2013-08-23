#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <algorithm>

template <typename T>
class Matrix1D 
{
public:
	Matrix1D()
	{
		size = 0;
		data = NULL;
	}

	Matrix1D(int size)
	{
		this->size = size;
		data = new T[size];
	}

	Matrix1D(const Matrix1D<T>& m)
	{
		size = m.size;
		if(size > 0)
		{
			data = new T[this->size];
			std::copy(m.data, m.data + size, data);
		}
		else
			data = NULL;
	}

	~Matrix1D()
	{
		if(data)
			delete data;
	}

	void Resize(int size, bool reserve = false)
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

	T& operator()(int i)
	{
		return data[i];
	}
private:
	T* data;
	int size;
};

#define Vector Matrix1D

template <typename T>
class  Matrix2D
{
public:
	Matrix2D()
	{
		size1 = 0;
		size2 = 0;
		data = NULL;
	}

	Matrix2D(int size1, int size2)
	{
		this->size1 = size1;
		this->size2 = size2;
		this->size = size1 * size2;
		data = new T[this->size];
	}

	Matrix2D(const Matrix2D<T>& m)
	{
		size1 = m.size1;
		size2 = m.size2;
		size = m.size;
		if(size > 0)
		{
			data = new T[this->size];
			std::copy(m.data, m.data + size, data);
		}
		else
			data = NULL;
	}

	~Matrix2D()
	{
		if(data)
			delete data;
	}

	void Resize(int size1, int size2)
	{
		if(data)
			delete data;
		this->size1 = size1;
		this->size2 = size2;
		this->size = size1*size2;
		data = new T[this->size];
	}

	int Size1() const
	{
		return size1;
	}

	int Size2() const
	{
		return size2;
	}

	T& operator()(int i, int j)
	{
		return data[i*size2 + j];
	}
private:
	T* data;
	int size;
	int size1;
	int size2;
};

template <typename T>
class Matrix3D
{
public:
	Matrix3D()
	{
		size = 0;
		size1 = 0;
		size2 = 0;
		size3 = 0;
		data = NULL;
	}

	Matrix3D(int size1, int size2, int size3)
	{
		this->size1 = size1;
		this->size2 = size2;
		this->size3 = size3;
		this->size = size1*size2*size3;
		this->size23 = size2*size3;
		data = new T[this->size];
	}

	Matrix3D(const Matrix3D<T>& m)
	{
		size1 = m.size1;
		size2 = m.size2;
		size3 = m.size3;
		size = m.size;
		if(size > 0)
		{
			data = new T[this->size];
			std::copy(data, data + size, m.data);
		}
		else
			data = NULL;
	}

	~Matrix3D()
	{
		if(data)
			delete data;
	}

	void Resize(int size1, int size2, int size3)
	{
		if(data)
			delete data;
		this->size1 = size1;
		this->size2 = size2;
		this->size3 = size3;
		this->size = size1*size2*size3;
		this->size23 = size2*size3;
		data = new T[this->size];
	}

	int Size1() const
	{
		return size1;
	}

	int Size2() const
	{
		return size2;
	}

	int Size3() const
	{
		return size3;
	}

	T& operator()(int i, int j, int k)
	{
		return data[i*size23 + j*size3 + k];
	}
private:
	T* data;
	int size;
	int size1;
	int size2;
	int size3;
	int size23; //size2 * size3
};
#endif
