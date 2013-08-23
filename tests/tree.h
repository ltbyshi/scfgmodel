#ifndef __TREE_H__
#define __TREE_H__

#include <cstddef>
#include <cstring>

#include "sequence.h"
#include "stack.h"

enum SYMBOL {
	SYMBOL_A,
	SYMBOL_U,
	SYMBOL_G,
	SYMBOL_C,
	SYMBOL_LP,
	SYMBOL_RP,
	SYMBOL_SS,
	SYMBOL_EPS
};

enum STATE {
	STATE_ANY,
	STATE_P,
	STATE_L,
	STATE_R,
	STATE_D,
	STATE_S,
	STATE_B,
	STATE_E
};

struct ParseTreeNode
{
	SYMBOL lsym;
	SYMBOL rsym;
	STATE state;
	ParseTreeNode* lchild;
	ParseTreeNode* rchild;
	bool visited;

	ParseTreeNode(SYMBOL lsym, SYMBOL rsym, STATE state)
	{
		this->lsym = lsym;
		this->rsym = rsym;
		this->state = state;
		lchild = NULL;
		rchild = NULL;
		visited = false;
	}

	ParseTreeNode()
	{
		lchild = NULL;
		rchild = NULL;
		state = STATE_ANY;
		visited = false;
	}

	~ParseTreeNode()
	{
		if(lchild)
			delete lchild;
		if(rchild)
			delete rchild;
	}
	
	bool HasChildren()
	{
		return rchild || lchild;
	}
};

class ParseTree
{
public:
	ParseTree()
	{
		root = new ParseTreeNode(SYMBOL_EPS, SYMBOL_EPS, STATE_S);
	}

	~ParseTree()
	{
		delete root;
	}

	ParseTreeNode* GetRoot()
	{
		return root;
	}

	struct ClearVisitedFlagVisitor
	{
		void operator()(ParseTreeNode* node)
		{
			node->visited = false;
		}
	};

	void ClearVisitedFlag()
	{
		PreOrder(ClearVisitedFlagVisitor());
	}
	
	template <class Visitor>
	void PreOrder(Visitor visit)
	{
		Stack<ParseTreeNode*> stk;
		stk.Push(root);
		while(!stk.Empty())
		{
			ParseTreeNode* node = stk.Pop();
			visit(node);
			if(node->lchild)
				stk.Push(node->lchild);
			if(node->rchild)
				stk.Push(node->rchild);
		}
	}

	void PostOrder(void (*visit)(ParseTreeNode*))
	{

	}

	Sequence<SYMBOL> GetSequence()
	{
		Sequence<SYMBOL> seq;
		Stack<ParseTreeNode*> stk;
		stk.Push(root);
		while(!stk.Empty())
		{
			ParseTreeNode* node = stk.Top();
			if(node->visited)
			{
				seq.Append(node->rsym);
				stk.Pop();
			}
			else
			{
				seq.Append(node->lsym);
				bool hasChildren = false;
				if(node->rchild)
				{
					hasChildren = true;
					node->visited = true;
					stk.Push(node->rchild);
				}
				if(node->lchild)
				{
					hasChildren = true;
					node->visited = true;
					stk.Push(node->lchild);
				}
				if(!hasChildren)
				{
					seq.Append(node->rsym);
					stk.Pop();
				}
			}
		}
		return seq;
	}

private:
	ParseTreeNode* root;
};

#endif
