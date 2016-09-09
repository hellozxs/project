#pragma once

#include <iostream>
using namespace std;
#include <vector>



//仿函数
template <typename T>
struct Less
{
	bool operator() (const T& l, const T& r)
	{
		return l < r;
	}
};


template <typename T>
struct Greater
{
	bool operator() (const T& l, const T& r)
	{
		return l>r;
	}
};

template <typename T, typename Cmp>
class Heap
{
public:
	Heap()
	{}
	Heap(T*arr, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			_a.push_back(arr[i]);
		}

		for (int i = (_a.size() - 2) / 2; i >= 0; i--)
		{
			AdjustDown(i);
		}
	}

	void PushBack(T data)
	{
		_a.push_back(data);
		AdjustUp(_a.size() - 1);

	}

	void PopFront()
	{
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		AdjustDown(0);
	}

	T Top()
	{
		if (_a.empty())
			throw;
		return _a[0];
	}

	size_t Size()
	{
		return _a.size();
	}

private:
	void AdjustUp(size_t pos)
	{
		if (pos < 0 || pos >= _a.size())
			throw;
		size_t child = pos;
		size_t parent = (pos - 1) / 2;
		Cmp cmp;
		while (child>0)
		{
			if (cmp(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}

	}

	void AdjustDown(int pos)
	{
		if (pos < 0)
			throw;
		size_t parent = pos;
		size_t child = pos * 2 + 1;

		while (child<_a.size())
		{
			Cmp cmp;
			size_t tmp = child;
			if (child + 1 < _a.size() && cmp(_a[child + 1], _a[child]))
				tmp = child + 1;

			if (cmp(_a[tmp], _a[parent]))
			{
				swap(_a[tmp], _a[parent]);
				parent = tmp;
				child = parent * 2 + 1;
			}
			else
				break;
		}

	}

private:
	vector<T>  _a;
};
