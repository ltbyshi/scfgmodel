#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>
#include <algorithm>
#include <fstream>

template <class T>
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
	
	void Fill(const T& value)
	{
		for(int i = 0; i < size; i ++)
			data[i] = value;
	}
	
	void Clear()
	{
		size1 = 0;
		size2 = 0;
		size = 0;
		data.clear();
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
	
	const T& operator()(int i, int j) const
	{
		return data[i*size2 + j];
	}
	//Save the contents to a text file
	void Dump(const char* fileName) const;
private:
	std::vector<T> data;
	int size;
	int size1;
	int size2;
};

template <class T>
void Matrix2D<T>::Dump(const char* fileName) const
{
	std::ofstream fout;
	fout.open(fileName, std::ios::out | std::ios::binary);
	if(!fout)
		return;
	int dim = 2;
	//Write file header
	fout.write(reinterpret_cast<const char*>(&dim), 4);
	fout.write(reinterpret_cast<const char*>(&size1), 4);
	fout.write(reinterpret_cast<const char*>(&size2), 4);
	//Write matrix values
	fout.seekp(32, std::ios::beg);
	for(int i = 0; i < size; i ++)
		fout.write(reinterpret_cast<const char*>(&data[i]), sizeof(T));
	fout.close();
}


//Three dimensional matrix
template <class T>
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
	
	void Fill(const T& value)
	{
		for(int i = 0; i < size;  i++)
			data[i] = value;
	}
	
	void Clear()
	{
		size1 = 0;
		size2 = 0;
		size3 = 0;
		size23 = 0;
		size = 0;
		data.clear();
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
	
	const T& operator()(int i, int j, int k) const
	{
		return data[i*size23 + j*size3 + k];
	}
	
	//Save the contents to a text file
	void Dump(const char* fileName) const;
	void DumpText(const char* fileName) const;
private:
	std::vector<T> data;
	int size;
	int size1;
	int size2;
	int size3;
	int size23; //size2 * size3
};

template <class T>
void Matrix3D<T>::Dump(const char* fileName) const
{
	std::ofstream fout;
	fout.open(fileName, std::ios::out | std::ios::binary);
	if(!fout)
		return;
	int dim = 3;
	//Write file header
	fout.write(reinterpret_cast<const char*>(&dim), 4);
	fout.write(reinterpret_cast<const char*>(&size1), 4);
	fout.write(reinterpret_cast<const char*>(&size2), 4);
	fout.write(reinterpret_cast<const char*>(&size3), 4);
	//Write matrix values
	fout.seekp(32, std::ios::beg);
	for(int i = 0; i < size; i ++)
		fout.write(reinterpret_cast<const char*>(&data[i]), sizeof(T));
	fout.close();
}

template <class T>
void Matrix3D<T>::DumpText(const char* fileName) const
{
	std::ofstream fout;
	fout.open(fileName);
	if(!fout)
		return;
	for(int i = 0; i < size; i ++)
		fout << data[i] << " ";
	fout.close();
}

#endif
