#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>

#include "utils.h"
#include "cmgraph.h"
#include "parsetree.h"
#include "stack.h"

using namespace std;

void RandomCMGraph(CMGraph& graph, int nNodes)
{
	for(int i = 0; i < nNodes; i ++)
		graph.CreateNode(Choose(ALLCMSTATES, NUMCMSTATES));
	for(int i = 0; i < nNodes; i ++)
	{
		for(int j = 0; j < nNodes / 2; j ++)
			graph.CreateEdge(i, j);
	}
}

void Test_CMGraph()
{
	ParseTree tree;
	tree.Parse("..(((..).)).((.(..))).");
	CMGraph graph;
	graph.FromParseTree(tree);
	PlotCMGraph("graph.dot", graph);
	system("dot -Tpdf -o graph.pdf graph.dot");
	PlotParseTree("tree.dot", tree);
	system("dot -Tpdf -o tree.pdf tree.dot");
}

int main()
{
	srand((unsigned int)time(0));
	Test_CMGraph();
	return 0;
}