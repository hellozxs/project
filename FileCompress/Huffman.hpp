#pragma once

#include "Heap.hpp"

using namespace std;

template <typename T>
struct HuffmanNode
{
	T _value;
	HuffmanNode<T>* _left;
	HuffmanNode<T>* _right;

	HuffmanNode(T value)
		:_value(value)
		, _left(NULL)
		, _right(NULL)
	{}
};

template <typename T>
class HuffmanTree
{
	typedef HuffmanNode<T>  Node;
public:
	Node * CreatTree(const T* arr, size_t size, const T& invalid)
	{
		//建小堆
		Heap<Node*, Cmp>hp;
		size_t i = 0;
		for (i = 0; i < size; i++)
		{
			if (arr[i] != invalid)
			{
				Node* tmp = new Node(arr[i]);
				hp.PushBack(tmp);
			}
		}

		//构建哈夫曼树
		while (hp.Size() >1)
		{
			Node* LeftChild = hp.Top();
			hp.PopFront();
			Node* RightChild = hp.Top();
			hp.PopFront();
			Node* parent = new Node(LeftChild->_value + RightChild->_value);
			hp.PushBack(parent);
			parent->_left = LeftChild;
			parent->_right = RightChild;
		}

		return hp.Top();
	}

private:
	
	struct Cmp
	{
		bool operator()(Node* l, Node* r)
		{
			return l->_value._count  <  r->_value._count;
		}
	};
	

private:
	Node* _root;

};

