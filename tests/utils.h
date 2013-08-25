#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdlib>
#include "arraytree.h"
#include "cmgraph.h"

void BuildPairTable(const char* structure, int length, int* table);

//print a formated message and terminate
void Die(const char* format, ...);
//draw a graph of the parse tree
//output in .dot format
void PlotParseTree(const char* filename, ParseTree& tree);
//draw a graph of the CM graph
//output in .dot format
void PlotCMGraph(const char* filename, CMGraph& graph);
//return a random element from choices
template <class T>
inline T Choose(const T* choices, int n)
{
	return choices[rand() % n];
}

#endif