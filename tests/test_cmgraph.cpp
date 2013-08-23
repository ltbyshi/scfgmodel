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
		int type = STATE_SS);
	)
		: graphNodes(graphNodes), treeNode(treeNode)
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
				graph.CreateEdge(SNode, IL);
				graph.CreateEdge(SNode, IR);
				//create insertion cycles
				graph.CreateEdge(IL, IL);
				graph.CreateEdge(IR, IR);
				//save expandes nodes
				node.graphNodes.clear();
				node.graphNodes.push_back(SNode);
				node.graphNodes.push_back(ILNode);
				node.graphNodes.push_back(IRNode);
			}
			
	}
}

void ExpandParseTree(ParseTree& tree, CMGraph& graph)
{
	vector<int> prevNodes;
	//stack storing tree nodes to be expanded
	Stack<ExpandedNode> S;
	S.Push(ExpandedNode(tree.GetRoot(),
						vector<int>(), STATE_SS));
	while(!S.Empty())
	{
		ExpandedNode& node = S.Top();
		
	}
}

void RandomCMGraph(CMGraph& graph, int nNodes)
{
	for(int i = 0; i < nNodes; i ++)
		graph.CreateNode(Choose(ALLCMSTATES, NUMCMSTATES));
}

void Test_CMGraph()
{
}

int main()
{
	srand((unsigned int)time(0));
	Test_CMGraph();
	return 0;
}