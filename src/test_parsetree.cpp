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
#include "parsetree.h"

#if 1

void Test_ParseTree()
{
	//ParseTree* tree = RandomParseTree(10);
	ParseTree tree;
	tree.Parse("..(((..).)).((.(..)))..(((..))).."
		"..(((..).)).((.(..)))..(((..)))..");
	
	PlotParseTree("tree.dot", tree);
	system("dot -Tpdf -o tree.pdf tree.dot");

	tree.ClearVisitedFlag();
	string seq = tree.GetSequence();

	cout << "sequence: ";
	for(size_t i = 0; i < seq.size(); i ++)
	{
		if(seq[i] != '-')
			cout << seq[i];
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
