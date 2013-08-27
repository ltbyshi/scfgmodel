#include "cmgraph.h"
#include "utils.h"

void CMGraph::FromParseTree(ParseTree& tree)
{
	//Clear all nodes if the graph is not empty
	if(Size() > 0)
		Clear();
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
		ExpandNode(tree, node);
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

void CMGraph::ExpandNode(ParseTree& tree,
						 ExpandedNode& expNode)
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
			MDNodes[0] = CreateNode(CMSTATE_S);
			nInsNodes = 2;
			InsNodes[0] = CreateNode(CMSTATE_IL);
			InsNodes[1] = CreateNode(CMSTATE_IR);
			break;
		case STATE_SL:
			nMDNodes = 1;
			MDNodes[0] = CreateNode(CMSTATE_S);
			nInsNodes = 0;
			break;
		case STATE_SR:
			nMDNodes = 1;
			MDNodes[0] = CreateNode(CMSTATE_S);
			nInsNodes = 1;
			InsNodes[0] = CreateNode(CMSTATE_IL);
			break;
		case STATE_L:
			nMDNodes = 2;
			MDNodes[0] = CreateNode(CMSTATE_ML);
			MDNodes[1] = CreateNode(CMSTATE_D);
			nInsNodes = 1;
			InsNodes[0] = CreateNode(CMSTATE_IL);
			break;
		case STATE_R:
			nMDNodes = 2;
			MDNodes[0] = CreateNode(CMSTATE_MR);
			MDNodes[1] = CreateNode(CMSTATE_D);
			nInsNodes = 1;
			InsNodes[0] = CreateNode(CMSTATE_IR);
			break;
		case STATE_P:
			nMDNodes = 4;
			MDNodes[0] = CreateNode(CMSTATE_ML);
			MDNodes[1] = CreateNode(CMSTATE_MR);
			MDNodes[2] = CreateNode(CMSTATE_MP);
			MDNodes[3] = CreateNode(CMSTATE_D);
			nInsNodes = 2;
			InsNodes[0] = CreateNode(CMSTATE_IL);
			InsNodes[1] = CreateNode(CMSTATE_IR);
			break;
		case STATE_B:
			nMDNodes = 1;
			MDNodes[0] = CreateNode(CMSTATE_B);
			nInsNodes = 0;
			break;
		case STATE_E:
			nMDNodes = 1;
			MDNodes[0] = CreateNode(CMSTATE_E);
			nInsNodes = 0;
			break;
		default:
			Die("Invalid state in parse tree: %s", StateName(treeNode.state));
	}
	//Connect MD nodes to ancestor nodes
	CreateEdges(expNode.ancestors, expNode.nAncestors,
					  MDNodes, nMDNodes);
	//Connect MD nodes to insertion nodes
	CreateEdges(MDNodes, nMDNodes,
					  InsNodes, nInsNodes);
	//Create insertion cycles
	for(int i = 0; i < nInsNodes; i ++)
		CreateCycle(InsNodes[i]);
	//Save expanded nodes
	expNode.nAncestors = nMDNodes + nInsNodes;
	for(int i = 0; i < nMDNodes; i ++)
		expNode.ancestors[i] = MDNodes[i];
	for(int i = 0; i < nInsNodes; i ++)
		expNode.ancestors[i + nMDNodes] = InsNodes[i];
}