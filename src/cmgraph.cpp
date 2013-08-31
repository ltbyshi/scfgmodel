#include "cmgraph.h"
#include "utils.h"

CMGraphNode::CMGraphNode()
{
	state = CMSTATE_ANY;
	for(int i = 0; i < NUMSYMBOLS; i ++)
		for(int j = 0; j < NUMSYMBOLS; j ++)
			ec[i][j] = 0.0;
}

int CMGraph::CreateNode(CMSTATE state)
{
	CMGraphNode newNode;
	newNode.state = state;
	nodes.push_back(newNode);
	return Size() - 1;
}

void CMGraph::CreateEdge(int parent, int child)
{
	nodes[parent].children.push_back(child);
	nodes[parent].tp.push_back(0.0);
	nodes[parent].tc.push_back(0.0);
	nodes[child].parents.push_back(parent);
}

void CMGraph::CreateEdges(int parent, 
					 const std::vector<int> children)
{
	for(size_t i = 0; i < children.size(); i ++)
		CreateEdge(parent, children[i]);
}

void CMGraph::CreateEdges(const std::vector<int> ancestors,
					 const std::vector<int> children)
{
	for(size_t i = 0; i < ancestors.size(); i ++)
		for(size_t j = 0; j < children.size(); j ++)
			CreateEdge(ancestors[i], children[j]);
}

void CMGraph::CreateEdges(const int* ancestors,
						int nAncestors,
						const int* children,
						int nChildren)
{
	for(int i = 0; i < nAncestors; i ++)
		for(int j = 0; j < nChildren; j ++)
			CreateEdge(ancestors[i], children[j]);
}

void CMGraph::InitParams()
{
	for(int i = 0; i < Size(); i ++)
	{
		CMGraphNode& node = nodes[i];
		//Initialize the number of emission symbols
		switch(node.state)
		{
			case CMSTATE_MP:
				node.nR = 1;
				node.nL = 1;
				break;
			case CMSTATE_IL:
			case CMSTATE_ML:
				node.nL = 1;
				node.nR = 0;
				break;
			case CMSTATE_IR:
			case CMSTATE_MR:
				node.nL = 0;
				node.nR = 1;
				break;
			default:
				node.nL = 0;
				node.nR = 0;
		}
		//Initialize emission probability
		switch(node.state)
		{
			//Emit a pair of symbols
			case CMSTATE_MP:
				node.ep[SYMBOL_A][SYMBOL_U] = 0.25;
				node.ep[SYMBOL_U][SYMBOL_A] = 0.25;
				node.ep[SYMBOL_G][SYMBOL_C] = 0.25;
				node.ep[SYMBOL_C][SYMBOL_G] = 0.25;
				break;
			//Emit a single symbol
			case CMSTATE_IL:
			case CMSTATE_ML:
			case CMSTATE_IR:
			case CMSTATE_MR:
				for(int s = 0; s < NUMSYMBOLS; s ++)
					node.ep[s][0] = 0.25;
				break;
			default:
				//Emit no symbols
				//All emission probability are 1
				for(int s = 0; s < NUMSYMBOLS; s ++)
					node.ep[s][0] = 1.0;
				break;
		}
		//Initialize transition probability
		//For B state, the probability is 1
		if(node.state == CMSTATE_B)
		{
			node.tp[0] = 1.0;
			node.tp[1] = 1.0;
		}
		else
		{
			//Probability from one state to each child state are equal
			for(int c = 0; c < node.Size(); c ++)
				node.tp[c] = 1.0 / node.Size();
		}
	}
}

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
	//Initialize parameters
	InitParams();
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
			Die("Invalid state in parse tree: %s", StateName[treeNode.state]);
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