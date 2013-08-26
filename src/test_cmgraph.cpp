#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>

#include "utils.h"
#include "cmgraph.h"
#include "arraytree.h"
#include "stack.h"

using namespace std;

#define MAX_EXPNODES	6
#define MAX_MDNODES		4
#define MAX_INSNODES	2

struct ExpandedNode
{
	//indices of the ancestor nodes in the CM graph
	int nAncestors;
	int ancestors[MAX_EXPNODES];
	//index of the expanded node in the parse tree
	int treeNode;
	
	ExpandedNode(int treeNode, 
			const int* ancestors,
			int nAncestors)
	{
		this->treeNode = treeNode;
		this->nAncestors = nAncestors;
		copy(ancestors, ancestors+nAncestors, this->ancestors);
	}
	
	ExpandedNode()
	{
	}
};

//Expand a node of a parse tree into CM states.
//The expanded nodes are saved to expNode.ancestors
//The tree node to be expanded is pass in expNode.treeNode
void ExpandNode(ParseTree& tree, 
				CMGraph& graph, ExpandedNode& expNode)
{
	ParseTreeNode& treeNode = tree[expNode.treeNode];
	int MDNodes[MAX_MDNODES];	//MD (match/deletion) nodes
	int nMDNodes = 0;	//number of MD nodes
	int InsNodes[MAX_INSNODES];	//insertion nodes
	int nInsNodes = 0; //number of insertino nodes
	
	//Create MD and insertion nodes based on node type
	switch(treeNode.state)
	{
		case STATE_SS:
			nMDNodes = 1;
			MDNodes[0] = graph.CreateNode(CMSTATE_S);
			nInsNodes = 2;
			InsNodes[0] = graph.CreateNode(CMSTATE_IL);
			InsNodes[1] = graph.CreateNode(CMSTATE_IR);
			break;
		case STATE_SL:
			nMDNodes = 1;
			MDNodes[0] = graph.CreateNode(CMSTATE_S);
			nInsNodes = 0;
			break;
		case STATE_SR:
			nMDNodes = 1;
			MDNodes[0] = graph.CreateNode(CMSTATE_S);
			nInsNodes = 1;
			InsNodes[0] = graph.CreateNode(CMSTATE_IL);
			break;
		case STATE_L:
			nMDNodes = 2;
			MDNodes[0] = graph.CreateNode(CMSTATE_ML);
			MDNodes[1] = graph.CreateNode(CMSTATE_D);
			nInsNodes = 1;
			InsNodes[0] = graph.CreateNode(CMSTATE_IL);
			break;
		case STATE_R:
			nMDNodes = 2;
			MDNodes[0] = graph.CreateNode(CMSTATE_MR);
			MDNodes[1] = graph.CreateNode(CMSTATE_D);
			nInsNodes = 1;
			InsNodes[0] = graph.CreateNode(CMSTATE_IR);
			break;
		case STATE_P:
			nMDNodes = 4;
			MDNodes[0] = graph.CreateNode(CMSTATE_ML);
			MDNodes[1] = graph.CreateNode(CMSTATE_MR);
			MDNodes[2] = graph.CreateNode(CMSTATE_MP);
			MDNodes[3] = graph.CreateNode(CMSTATE_D);
			nInsNodes = 2;
			InsNodes[0] = graph.CreateNode(CMSTATE_IL);
			InsNodes[1] = graph.CreateNode(CMSTATE_IR);
			break;
		case STATE_B:
			nMDNodes = 1;
			MDNodes[0] = graph.CreateNode(CMSTATE_B);
			nInsNodes = 0;
			break;
		case STATE_E:
			nMDNodes = 1;
			MDNodes[0] = graph.CreateNode(CMSTATE_E);
			nInsNodes = 0;
			break;
		default:
			Die("Invalid state in parse tree!");
	}
	//Connect MD nodes to ancestor nodes
	graph.CreateEdges(expNode.ancestors, expNode.nAncestors,
					  MDNodes, nMDNodes);
	//Connect MD nodes to insertion nodes
	graph.CreateEdges(MDNodes, nMDNodes,
					  InsNodes, nInsNodes);
	//Create insertion cycles
	for(int i = 0; i < nInsNodes; i ++)
		graph.CreateCycle(InsNodes[i]);
	//Save expanded nodes
	expNode.nAncestors = nMDNodes + nInsNodes;
	for(int i = 0; i < nMDNodes; i ++)
		expNode.ancestors[i] = MDNodes[i];
	for(int i = 0; i < nInsNodes; i ++)
		expNode.ancestors[i + nMDNodes] = InsNodes[i];
}

void ExpandParseTree(ParseTree& tree, CMGraph& graph)
{
	//stack storing tree nodes to be expanded
	Stack<ExpandedNode> S;
	//push the root node into stack
	ExpandedNode rootNode;
	rootNode.treeNode = tree.GetRoot();
	rootNode.nAncestors = 0;
	S.Push(rootNode);
	while(!S.Empty())
	{
		ExpandedNode& node = S.Top();
		ExpandNode(tree, graph, node);
		if(tree[node.treeNode].state == STATE_B)
		{
			//copy the node as left branch
			ExpandedNode leftNode(node);
			leftNode.treeNode = tree[node.treeNode].lchild;
			//push right branch
			node.treeNode = tree[node.treeNode].rchild;
			//push left branch
			S.Push(leftNode);
		}
		else if(tree[node.treeNode].state == STATE_E)
			S.Pop();
		else
			node.treeNode = tree[node.treeNode].lchild;
	}
}

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
	tree.Parse("..(((..).)).((.(..)))."
			"..(((..).)).((.(..))).");
	CMGraph graph;
	ExpandParseTree(tree, graph);
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