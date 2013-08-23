#include <iostream>
#include <ctime>
#include <vector>

#include "utils.h"
#include "cmgraph.h"
#include "arraytree.h"
#include "stack.h"

using namespace std;

#define STATE_SL	10	//left S node
#define STATE_SR	11	//right S node
#define STATE_SS	12	//start S node

struct ExpandedNode
{
	//indices of the expanded nodes in the CM graph
	vector<int> graphNodes;
	//index of the expanded node in the parse tree
	int treeNode;
	//node type for S node
	//values: 
	int type;
	
	ExpandedNode(int treeNode, vector<int>& graphNodes,
		int type = STATE_SS)
		: graphNodes(graphNodes), treeNode(treeNode)
		{
		}
	
	ExpandedNode()
	{
	}
};

void ExpandNode(ParseTree& tree, 
				CMGraph& graph, ExpandedNode& node)
{
	ParseTreeNode& treeNode = tree[node.treeNode];
	switch(treeNode.state)
	{
		case STATE_S:
			if(node.type == STATE_SS)
			{
				//create nodes
				int SNode = graph.CreateNode(CMSTATE_S);
				int ILNode = graph.CreateNode(CMSTATE_IL);
				int IRNode = graph.CreateNode(CMSTATE_IR);
				//connect insertion nodes
				graph.CreateEdge(SNode, ILNode);
				graph.CreateEdge(SNode, IRNode);
				//create insertion cycles
				graph.CreateEdge(ILNode, ILNode);
				graph.CreateEdge(IRNode, IRNode);
				//save expandes nodes
				node.graphNodes.clear();
				node.graphNodes.push_back(SNode);
				node.graphNodes.push_back(ILNode);
				node.graphNodes.push_back(IRNode);
			}
			else if(node.type == STATE_SL)
			{
				int SNode = graph.CreateNode(CMSTATE_S);
				graph.CreateEdge(node.graphNodes[0], SNode);
				//save expandes nodes
				node.graphNodes.clear();
				node.graphNodes.push_back(SNode);
			}
			else if(node.type == STATE_SR)
			{
				int SNode = graph.CreateNode(CMSTATE_S);
				int ILNode = graph.CreateNode(CMSTATE_IL);
				graph.CreateEdge(node.graphNodes[0], SNode);
				graph.CreateEdge(SNode, ILNode);
				graph.CreateEdge(ILNode, ILNode);
				//save expanded nodes
				node.graphNodes.clear();
				node.graphNodes.push_back(SNode);
				node.graphNodes.push_back(ILNode);
			}
			else
			{
			}
			break;
		case STATE_L:
		case STATE_P:
		case STATE_E:
		case STATE_E:
		default:
	}
}

void ExpandParseTree(ParseTree& tree, CMGraph& graph)
{
	vector<int> prevNodes;
	//stack storing tree nodes to be expanded
	Stack<ExpandedNode> S;
	S.Push(ExpandedNode(tree.GetRoot(),
						prevNodes, STATE_SS));
	while(!S.Empty())
	{
		ExpandedNode& node = S.Top();
		ExpandNode(tree, graph, node);
		if(tree[node.treeNode].state == STATE_B)
		{
			//push right branch
			node.type = STATE_SR;
			//push left branch
			ExpandedNode leftNode(node);
			leftNode.type = STATE_SL;
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
	CMGraph graph;
	RandomCMGraph(graph, 10);
	PlotCMGraph("graph.dot", graph);
	system("dot -Tpdf -o graph.pdf graph.dot");
}

int main()
{
	srand((unsigned int)time(0));
	Test_CMGraph();
	return 0;
}