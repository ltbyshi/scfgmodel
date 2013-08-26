#ifndef __CMGRAPH_H__
#define __CMGRAPH_H__

#include <vector>
#include "symbol.h"

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

class CMGraph
{
public:
	CMGraph()
	{
		size = 0;
	}
	
	~CMGraph()
	{
	}
	
	int Size() const
	{
		return size;
	}
	
	int GetRoot() const
	{
		return 0;
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
		size ++;
		return size - 1;
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
		for(int i = 0; i < size; i ++)
			visitor(i);
	}
	
	//EdgeVisitor must overload () operator:
	//void operator()(int parent, int child)
	template <class EdgeVisitor>
	void TraverseEdges(EdgeVisitor& visitor)
	{
		for(int i = 0; i < size; i ++)
			for(size_t j = 0; j < nodes[i].children.size(); j ++)
				visitor(i, nodes[i].children[j]);
	}
private:
	std::vector<CMGraphNode> nodes;
	int size;
};

#endif