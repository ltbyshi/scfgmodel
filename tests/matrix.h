#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>
#include <algorithm>

template <typename T>
class  Matrix2D
{
public:
	Matrix2D()
	{
		size1 = 0;
		size2 = 0;
		size = 0;
	}

	Matrix2D(int size1, int size2)
	{
		Resize(size1, size2);
	}


	~Matrix2D()
	{
	}

	void Resize(int size1, int size2)
	{
		this->size1 = size1;
		this->size2 = size2;
		this->size = size1*size2;
		data.resize(this->size);
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
	
	T& operator()(int i, int j) const
	{
		return data[i*size2 + j];
	}
private:
	std::vector<T> data;
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
	}

	Matrix3D(int size1, int size2, int size3)
	{
		Resize(size1, size2, size3);
	}

	~Matrix3D()
	{
	}

	void Resize(int size1, int size2, int size3)
	{
		this->size1 = size1;
		this->size2 = size2;
		this->size3 = size3;
		this->size = size1*size2*size3;
		this->size23 = size2*size3;
		data.resize(this->size);
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
	
	T& operator()(int i, int j, int k) const
	{
		return data[i*size23 + j*size3 + k];
	}
private:
	std::vector<T> data;
	int size;
	int size1;
	int size2;
	int size3;
	int size23; //size2 * size3
};
#endif
