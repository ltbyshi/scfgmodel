#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdarg>
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

void Die(const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	cerr << "Error: " << buffer << endl;
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
			<< tree[parent].lsym << "|<st> "
			<< StateName[tree[parent].state] << "|<rc> "
			<< tree[parent].rsym << "\"];" << endl;
		os << "\tNode_" << child << " [label=\"<lc> "
			<< tree[child].lsym << "|<st> "
			<< StateName[tree[child].state] << "|<rc> "
			<< tree[child].rsym << "\"];" << endl;
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

vector<SYMBOL> StrToSeq(const char* seq)
{
	size_t len = strlen(seq);
	vector<SYMBOL> newSeq(len);
	for(size_t i = 0; i < len; i ++)
	{
		switch(seq[i])
		{
			case 'A':
				newSeq[i] = SYMBOL_A;
				break;
			case 'U':
				newSeq[i] = SYMBOL_U;
				break;
			case 'G':
				newSeq[i] = SYMBOL_G;
				break;
			case 'C':
				newSeq[i] = SYMBOL_C;
				break;
			default:
				Die("Unrecognized character in RNA sequence: %c", seq[i]);
		}
	}
	return newSeq;
}

std::string SeqToStr(const std::vector<SYMBOL>& seq)
{
	string str;
	for(size_t i = 0; i < seq.size(); i ++)
		str += SymbolName[seq[i]];
	return str;
}

void PlotCMGraph(const char* filename, CMGraph& graph)
{
	ofstream fout(filename);
	if(!fout)
		Die("Cannot open the file");
	fout << "digraph{" << endl;
	fout << "\tnode [shape=record,"
		"fontname=\"Arial\","
		"fontsize = 12.0];" << endl;
	fout << "\tedge [fontsize = 8.0];" << endl;
	//Write nodes
	fout << "//Nodes" << endl;
	for(int i = 0; i < graph.Size(); i ++)
		fout << "\t" << CMStateName[graph[i].state]
			<< "_" << i << " [label=\""
			<< CMStateName[graph[i].state] 
			<< "\"];" << endl;
	//Write Edges
	fout << "//Edges" << endl;
	fout.precision(2);
	for(int i = 0; i < graph.Size(); i ++)
	{
		CMGraphNode& node = graph[i];
		for(int c = 0; c < node.Size(); c ++)
			fout << "\t" << CMStateName[node.state]
				<< "_" << i << " -> "
				<< CMStateName[graph[node[c]].state]
				<< "_" << node[c] << " [label=\""
				<< scientific << node.tp[c]
				<< "\"];" << endl;
	}
	fout << "}";
	fout.close();
}