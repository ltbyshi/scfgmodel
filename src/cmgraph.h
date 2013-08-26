#ifndef __CMGRAPH_H__
#define __CMGRAPH_H__

#include <vector>
#include "symbol.h"

struct CMGraphNode
{
	CMSTATE state;
	std::vector<int> children;
	//probability of emitting left and right symbols
	PRECISION emitL[NUMSYMBOLS];
	PRECISION emitR[NUMSYMBOLS];
	
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