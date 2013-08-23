#ifndef __TREE_H__
#define __TREE_H__

#include "vector.h"
#include "stack.h"

struct ParseTreeNode
{
	STATE state;
	SYMBOL lsym;
	SYMBOL rsym;
	int nNodes;
	ParseTreeNode* lchild;
	ParseTreeNode* rchild;

	ParseTreeNode()
	{
		this->lchild = NULL;
		this->rchild = NULL;
		nNodes = 0;
	}

	ParseTreeNode(STATE state, SYMBOL lsym, SYMBOL rsym)
	{
		this->state = state;
		this->lsym = lsym;
		this->rsym = rsym;
		this->lchild = NULL;
		this->rchild = NULL;
		nNodes = 0;
	}

	~ParseTreeNode()
	{
		if(lchild)
			delete lchild;
		if(rchild)
			delete rchild;
	}

	void AddSingle(ParseTreeNode* node)
	{
		lchild = left;
		nNodes = 1;
	}

	void AddBranch(ParseTreeNode* left, ParseTreeNode* right)
	{
		lchild = left;
		rchild = right;
		nNodes = 2;
	}
};

class ParseTree
{
public:
	ParseTree()
	{
		root = new ParseTreeNode(STATE_S, SYMBOL_EPS, SYMBOL_EPS);
	}

	~ParseTree()
	{
		if(root)
			delete root;
	}
	//traverse the parse tree and get the sequence
	Sequence<SYMBOL> Sequence() const
	{
		Sequence<SYMBOL> seq;
		//stack that store previously visited node
		Stack<ParseTreeNode*> stk;
		stk.Push(root);
		while(!stk.Empty())
		{
			ParserTreeNode* node = stk.Pop();
			if(node->lchild)
			//read left symbol
			seq.Append(node->lsym);
			//dive more deeply
			stk.Push(node->lchild);
			stk.Push(node->rchild);
		}
	}

private:
	ParseTreeNode* root;
};
#endif
