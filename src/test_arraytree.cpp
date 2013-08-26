#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstring>

using namespace std;

#include "queue.h"
#include "utils.h"
#include "arraytree.h"

#if 1
void  RandomNode(ParseTreeNode* node)
{
	node->state = Choose(ALLSTATES, NUMSTATES);
	node->lsym = Choose(ALLSYMBOLS, NUMSYMBOLS);
	node->rsym = Choose(ALLSYMBOLS, NUMSYMBOLS);
}

ParseTree* RandomParseTree(int nNodes)
{
	ParseTree* tree = new ParseTree;
	int root = 0;

	Queue<int> Q;
	Q.Push(root);
	for(int i = 0; i < nNodes; i ++)
	{
		int node = Q.Pop();
		ParseTreeNode newNode;
		RandomNode(&newNode);
		int lchild = tree->CreateLChild(node, newNode.lsym,
			newNode.rsym, newNode.state);
		RandomNode(&newNode);
		int rchild = tree->CreateRChild(node, newNode.lsym,
			newNode.rsym, newNode.state);
		Q.Push(lchild);
		Q.Push(rchild);
	}
	
	return tree;
}

void Test_ParseTree()
{
	//ParseTree* tree = RandomParseTree(10);
	ParseTree tree;
	tree.Parse("..(((..).)).((.(..)))..(((..))).."
		"..(((..).)).((.(..)))..(((..)))..");
	
	PlotParseTree("tree.dot", tree);
	system("dot -Tpdf -o tree.pdf tree.dot");

	tree.ClearVisitedFlag();
	Sequence<SYMBOL> seq = tree.GetSequence();

	cout << "sequence: ";
	for(int i = 0; i < seq.Size(); i ++)
	{
		if(seq[i] != SYMBOL_EPS)
			cout << SymbolName(seq[i]);
	}
	cout << endl;
}
#endif

int main()
{
	srand((unsigned int)time(0));
	//Test_Queue();
	Test_ParseTree();
	return 0;
}
