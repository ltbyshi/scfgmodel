#ifndef __CMGRAPH_H__
#define __CMGRAPH_H__

#include <vector>
#include "symbol.h"

struct CMGraphNode
{
	CMSTATE state;
	std::vector<int> children;
	
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
	
	CMGraphNode(const CMGraphNode& node)
		: state(node.state), 
		children(node.children)
		{
		}
};

class CMGraph
{
public:
	CMGraph()
	{
		CreateNode(CMSTATE_ANY);
		size = 1;
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
		for(size_t i = 0; i < size; i ++)
			for(size_t j = 0; j < nodes[i].children.size(); j ++)
				visitor(i, j);
	}
private:
	std::vector<CMGraphNode> nodes;
	int size;
};

#endif