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
	std::vector<int> children;
	//transition probability
	std::vector<PRECISION> tp;
	//emission probability
	PRECISION ep[NUMSYMBOLS][NUMSYMBOLS];
	//number of left/right symbols to emit
	int nL, nR;
	
	int Size() const
	{
		return children.size();
	}
	
	CMGraphNode()
		: state(CMSTATE_ANY)
		{
		}
	
	CMGraphNode(CMSTATE state)
		: state(state)
		{
		}
	//Return the index of a child node
	int GetChild(int i) const
	{
		return children[i];
	}
	//Return the index of a child node
	int operator[](int i) const
	{
		return children[i];
	}
	//Return transition probability from the state
	// to state i
	PRECISION& TransProb(int i)
	{
		return tp[i];
	}
	//Return emission probability for pair s1, s2
	PRECISION& EmitProb(SYMBOL s1, SYMBOL s2)
	{
		return ep[int(s1)][int(s2)];
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
	int CreateNode(CMSTATE state)
	{
		nodes.push_back(CMGraphNode(state));
		return Size() - 1;
	}
	
	void CreateEdge(int parent, int child)
	{
		nodes[parent].children.push_back(child);
	}
	//Create an edge between parent and each children
	void CreateEdges(int parent, 
					 const std::vector<int> children)
	{
		for(size_t i = 0; i < children.size(); i ++)
			CreateEdge(parent, children[i]);
	}
	//Create an edge between each ancestor and each child
	void CreateEdges(const std::vector<int> ancestors,
					 const std::vector<int> children)
	{
		for(size_t i = 0; i < ancestors.size(); i ++)
			for(size_t j = 0; j < children.size(); j ++)
				CreateEdge(ancestors[i], children[j]);
	}
	//Create an edge between each ancestor and each children
	void CreateEdges(const int* ancestors, int nAncestors,
					 const int* children, int nChildren)
	{
		for(int i = 0; i < nAncestors; i ++)
			for(int j = 0; j < nChildren; j ++)
				CreateEdge(ancestors[i], children[j]);
	}
	//Create an edge to itself
	void CreateCycle(int node)
	{
		CreateEdge(node, node);
	}
	
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