#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

#include "utils.h"
#include "stack.h"

void BuildPairTable(const char* structure, int length, int* table)
{
	Stack<int> S;
	for(int i = 0; i < length; i ++)
	{
		switch(structure[i])
		{
			case '(':
				S.Push(i);
				break;
			case '.':
				table[i] = -1;
				break;
			case ')':
				if(S.Empty())
					Die("Brackets do not match");
				table[S.Top()] = i;
				table[i] = S.Pop();
				break;
			default:
				Die("Junk character found in RNA structure");
		}
	}
}

void Die(const char* message)
{
	cerr << "Error: " << message << endl;
	exit(-1);
}

struct TreeWriter
{
	ostream& os;
	ParseTree& tree;
	
	TreeWriter(ostream& os, ParseTree& tree)
		:os(os), tree(tree)
	{
	}
	
	void WriteEdge(int parent, int child)
	{
		
		os << "\tNode_" << parent << " [label=\"<lc> "
			<< SymbolName(tree[parent].lsym) << "|<st> "
			<< StateName(tree[parent].state) << "|<rc> "
			<< SymbolName(tree[parent].rsym) << "\"];" << endl;
		os << "\tNode_" << child << " [label=\"<lc> "
			<< SymbolName(tree[child].lsym) << "|<st> "
			<< StateName(tree[child].state) << "|<rc> "
			<< SymbolName(tree[child].rsym) << "\"];" << endl;
		os << "\tNode_" << parent << ":st"
			<< " -> Node_" << child << ":st"
			<< ";" << endl;
	}
	
	void operator()(int node)
	{
		if(tree.HasLChild(node))
			WriteEdge(node, tree.GetLChild(node));
		if(tree.HasRChild(node))
			WriteEdge(node, tree.GetRChild(node));
	}
};

void PlotParseTree(const char* filename, ParseTree& tree)
{
	ofstream fout(filename);
	if(!fout)
		Die("Cannot open the file");

	fout << "digraph{" << endl;
	fout << "\tnode [shape=record,"
		"fontname=\"Arial\","
		"fontsize = 12.0];" << endl;
	TreeWriter writer(fout, tree);
	tree.PreOrder(writer);
	fout << "}";
	fout.close();
}

struct GraphWriter
{
	ostream& os;
	CMGraph& graph;
	
	GraphWriter(ostream& os, CMGraph& graph)
		: os(os), graph(graph)
		{
		}
	
	void operator()(int parent, int child)
	{
		os << "\tNode_" << parent << " [label=\""
			<< CMStateName(graph[parent].state)
			<< "\"];" << endl;
		os << "\tNode_" << child << " [label=\""
			<< CMStateName(graph[child].state)
			<< "\"];" << endl;
		os << "\tNode_" << parent
			<< " -> Node_" << child
			<< ";" << endl;
	}
};

void PlotCMGraph(const char* filename, CMGraph& graph)
{
	ofstream fout(filename);
	if(!fout)
		Die("Cannot open the file");
	fout << "digraph{" << endl;
	fout << "\tnode [shape=record,"
		"fontname=\"Arial\","
		"fontsize = 12.0];" << endl;
	GraphWriter writer(fout, graph);
	graph.TraverseEdges(writer);
	fout << "}";
	fout.close();
}