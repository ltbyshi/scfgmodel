#ifndef __ARRAYTREE_H__
#define __ARRAYTREE_H__

#include <cassert>
#include <cstring>

#include "sequence.h"
#include "stack.h"
#include "symbol.h"
#include "vector.h"

struct ParseTreeNode
{
	int lchild;
	int rchild;
	bool visited;
	SYMBOL lsym;
	SYMBOL rsym;
	STATE state;
};

class ParseTree
{
public:
	ParseTree()
	{
		size = 1;
		ParseTreeNode& root = nodes[0];
		root.lchild = -1;
		root.rchild = -1;
		root.lsym = SYMBOL_EPS;
		root.rsym = SYMBOL_EPS;
		root.state = STATE_SS;
	}
	
	~ParseTree()
	{
	}
	
	int GetRoot()
	{
		return 0;
	}
	
	ParseTreeNode* GetNode(int i)
	{
		return &(nodes[i]);
	}
	
	bool IsValid(int i) const
	{
		return ((0 <= i) && (i < size));
	}
	
	//get node by index
	ParseTreeNode& operator[](int i)
	{
		assert(IsValid(i));
		return nodes[i];
	}
	
	ParseTreeNode& operator[](int i) const
	{
		assert(IsValid(i));
		return nodes[i];
	}
	
	int Size() const
	{
		return nodes.Size();
	}
	
	void ClearVisitedFlag()
	{
		for(int i = 0; i < size; i ++)
			nodes[i].visited = false;
	}
	
	int CreateLChild(int parent, SYMBOL lsym, SYMBOL rsym, STATE state)
	{
		assert(IsValid(parent));
		int child = CreateNode(lsym, rsym, state);
		nodes[parent].lchild = child;
		return child;
	}
	
	int CreateRChild(int parent, SYMBOL lsym, SYMBOL rsym, STATE state)
	{
		assert(IsValid(parent));
		int child = CreateNode(lsym, rsym, state);
		nodes[parent].rchild = child;
		return child;
	}
	
	bool HasLChild(int parent) const
	{
		assert(IsValid(parent));
		return (nodes[parent].lchild >= 0);
	}
	
	bool HasRChild(int parent) const
	{
		assert(IsValid(parent));
		return (nodes[parent].rchild >= 0);
	}
	
	int GetLChild(int parent)
	{
		assert(IsValid(parent));
		return nodes[parent].lchild;
	}
	
	int GetRChild(int parent)
	{
		assert(IsValid(parent));
		return nodes[parent].rchild;
	}
	
	void SetLChild(int parent, int child)
	{
		assert(IsValid(parent));
		nodes[parent].lchild = child;
	}
	
	void SetRChild(int parent, int child)
	{
		assert(IsValid(parent));
		nodes[parent].rchild = child;
	}
	
	void RemoveAll()
	{
		size = 1;
	}
	
	//NodeVisitor must overload () operator:
	//void operator()(int node)
	template <class NodeVisitor>
	void PreOrder(NodeVisitor& visitor)
	{
		Stack<int> S;
		S.Push(0);
		while(!S.Empty())
		{
			int node = S.Pop();
			visitor(node);
			if(HasRChild(node))
				S.Push(nodes[node].rchild);
			if(HasLChild(node))
				S.Push(nodes[node].lchild);
		}
	}
	
	Sequence<SYMBOL> GetSequence();
	
	//build a parse tree from RNA structure
	void Parse(const char* structure);
	
private:
	//Return the index of the new node
	int CreateNode(SYMBOL lsym, SYMBOL rsym, STATE state)
	{
		if(size >= nodes.Size())
			nodes.Resize(nodes.Size()*2, true);
		nodes[size].lsym = lsym;
		nodes[size].rsym = rsym;
		nodes[size].state = state;
		nodes[size].rchild = -1;
		nodes[size].lchild = -1;
		size ++;
		return size - 1;
	}
private:
	int size;
	Vector<ParseTreeNode> nodes;
};
#endif