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

void Test_NodeOrder(CMGraph& graph)
{
	for(int i = 0; i < graph.Size(); i ++)
	{
		CMGraphNode& node = graph[i];
		for(int c = 0; c < node.Size(); c ++)
		{
			if(node[c] < i)
				cout << "[NodeOrderCheck]: "
					<< node[c] << " < "
					<< i << endl;
		}
	}
}

void Test_CMGraph()
{
	ParseTree tree;
	tree.Parse("..(((..).)).((.(..))).");
	CMGraph graph;
	graph.FromParseTree(tree);
	
	Test_NodeOrder(graph);
	graph.Dump("graph.txt");
	
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