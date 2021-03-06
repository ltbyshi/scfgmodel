#ifndef __CMGRAPH_H__
#define __CMGRAPH_H__

#include <vector>
#include <algorithm>
#include "symbol.h"
#include "parsetree.h"

#define MAX_EXPNODES	6
#define MAX_MDNODES		4
#define MAX_INSNODES	2

struct CMGraphNode
{
	CMSTATE state;
	//indices of parents in the CM graph
	std::vector<int> parents;
	//indices of children in the CM graph
	std::vector<int> children;
	//transition probability
	std::vector<PRECISION> tp;
	//emission probability
	PRECISION ep[NUMSYMBOLS][NUMSYMBOLS];
	//tc[c]: expected number of transitions from this state to c
	std::vector<PRECISION> tc;
	//Expected number of emissions from this state
	//If a pair (a, b) is emitted, the number is ec[a][b]
	//If a single symbol a is emitted, the number is ec[a][0]
	PRECISION ec[NUMSYMBOLS][NUMSYMBOLS];
	//Expected total number of transitions from this state
	PRECISION Ttot;
	//Expected total number of emissions from this state
	PRECISION Etot;
	//number of left/right symbols to emit
	int nL, nR;
	
	CMGraphNode();
	
	int Size() const
	{
		return children.size();
	}
	
	//Return the index of a child node
	int operator[](int i) const
	{
		return children[i];
	}
};

//Node to be expanded to from a node in a parse tree
//Used in to build a CM graph from a parse tree
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
		std::copy(ancestors, ancestors+nAncestors, this->ancestors);
	}
	
	ExpandedNode()
	{
	}
};

class CMGraph
{
public:
	CMGraph()
	{
	}
	
	~CMGraph()
	{
	}
	
	int Size() const
	{
		return nodes.size();
	}
	
	//build a CM graph from a parse tree
	void FromParseTree(ParseTree& tree);
	//get index of the root node
	int GetRoot() const
	{
		return 0;
	}
	//Clear all contents
	void Clear()
	{
		nodes.clear();
	}
	//access graph node by index
	CMGraphNode& operator[](int i)
	{
		return nodes[i];
	}
	
	const CMGraphNode& operator[](int i) const
	{
		return nodes[i];
	}
	//return the index of the created node
	int CreateNode(CMSTATE state);
	
	void CreateEdge(int parent, int child);
	//Create an edge between parent and each children
	void CreateEdges(int parent, 
					 const std::vector<int> children);
	//Create an edge between each ancestor and each child
	void CreateEdges(const std::vector<int> ancestors,
					 const std::vector<int> children);
	//Create an edge between each ancestor and each children
	void CreateEdges(const int* ancestors, int nAncestors,
					 const int* children, int nChildren);
	//Create an edge to itself
	void CreateCycle(int node)
	{
		CreateEdge(node, node);
	}
	//Save the CM graph to a file
	void Dump(const char* fileName);
	//Initialize all parameters to default
	void InitParams();
	//NodeVisitor must overload () operator:
	//void operator()(int node)
	template <class NodeVisitor>
	void TraverseNodes(NodeVisitor& visitor)
	{
		for(int i = 0; i < Size(); i ++)
			visitor(i);
	}
	
	//EdgeVisitor must overload () operator:
	//void operator()(int parent, int child)
	template <class EdgeVisitor>
	void TraverseEdges(EdgeVisitor& visitor)
	{
		for(int i = 0; i < Size(); i ++)
			for(size_t j = 0; j < nodes[i].children.size(); j ++)
				visitor(i, nodes[i].children[j]);
	}
private:
	//Expand a node of a parse tree into CM states.
	//The expanded nodes are saved to expNode.ancestors
	//The tree node to be expanded is pass in expNode.treeNode
	void ExpandNode(ParseTree& tree, ExpandedNode& expNode);
private:
	std::vector<CMGraphNode> nodes;
};

#endif