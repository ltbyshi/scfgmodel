#ifndef __ARRAYTREE_H__
#define __ARRAYTREE_H__

#include <cassert>
#include <cstring>
#include <vector>

#include <string>
#include "stack.h"
#include "symbol.h"
#include "vector.h"

struct ParseTreeNode
{
	int lchild;
	int rchild;
	bool visited;
	char lsym;
	char rsym;
	STATE state;
};

class ParseTree
{
public:
	ParseTree()
	{
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
	
	//get node by index
	ParseTreeNode& operator[](int i)
	{
		return nodes[i];
	}
	
	const ParseTreeNode& operator[](int i) const
	{
		return nodes[i];
	}
	
	int Size() const
	{
		return nodes.size();
	}
	
	void ClearVisitedFlag()
	{
		for(int i = 0; i < Size(); i ++)
			nodes[i].visited = false;
	}
	
	int CreateLChild(int parent, char lsym, char rsym, STATE state)
	{
		int child = CreateNode(lsym, rsym, state);
		nodes[parent].lchild = child;
		return child;
	}
	
	int CreateRChild(int parent, char lsym, char rsym, STATE state)
	{
		int child = CreateNode(lsym, rsym, state);
		nodes[parent].rchild = child;
		return child;
	}
	
	bool HasLChild(int parent) const
	{
		return (nodes[parent].lchild >= 0);
	}
	
	bool HasRChild(int parent) const
	{
		return (nodes[parent].rchild >= 0);
	}
	
	int GetLChild(int parent)
	{
		return nodes[parent].lchild;
	}
	
	int GetRChild(int parent)
	{
		return nodes[parent].rchild;
	}
	
	void SetLChild(int parent, int child)
	{
		nodes[parent].lchild = child;
	}
	
	void SetRChild(int parent, int child)
	{
		nodes[parent].rchild = child;
	}
	
	void Clear()
	{
		nodes.clear();
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
	
	std::string GetSequence();
	
	//build a parse tree from RNA structure
	void Parse(const char* structure);
	
private:
	//Return the index of the new node
	int CreateNode(char lsym, char rsym, STATE state)
	{
		ParseTreeNode newNode;
		newNode.lsym = lsym;
		newNode.rsym = rsym;
		newNode.state = state;
		newNode.rchild = -1;
		newNode.lchild = -1;
		nodes.push_back(newNode);
		
		return Size() - 1;
	}
private:
	std::vector<ParseTreeNode> nodes;
};
#endif