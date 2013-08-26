#include <cstring>

#include "grammar.h"
#include "stack.h"

SCFGModel::SCFGModel()
{
}

SCFGModel::~SCFGModel()
{
}

void SCFGModel::Build(const char* structure)
{
	//build a parse tree from the RNA structure
	tree.Parse(structure);
	//build a CM graph from the parse tree
	graph.FromParseTree(tree);
	
}

void SCFGModel::Inside(const vector<SYMBOL>& seq)
{
	int L = seq.size();	//sequence length
	int M = graph.Size();	//number of states
	alpha.Resize(L + 2, L + 1, M);
	//Initialization
	for(int j = 0; j <= L; j ++)
	{
		for(int v = M - 1; v >= 1; v --)
		{
			CMSTATE sv = graph[v].state;
			if(sv == CMSTATE_E)
			{
				alpha(v, j + 1, j) = 1.0;
			}
			else if(sv == CMSTATE_S
				|| sv == CMSTATE_D)
			{
				alpha(v, j + 1, j) = 0.0;
				for(int c = 0; c < graph[v].Size(); c ++)
					alpha(v, j + 1, j) +=
						graph[v].TransProb(v, graph[v][c])
						* alpha(graph[v][c], j + 1, j);
			}
			else if(sv == CMSTATE_B)
			{
				int lc = graph[v][0];
				int rc = graph[v][1];
				alpha(v, j + 1, j) = alpha(lc, j + 1, j)
					* alpha(rc, j + 1, j);
			}
			else
			{
				//MP, ML, MR, IL, IR
				alpha(v, j + 1, j) = 0.0;
			}
		}
	}
	//Recursion
	for(int j = 1; j <= L; j ++)
	{
		for(int i = j; j >= 1; i --)
		{
			for(int v = m - 1; v >= 1; v --)
			{
				CMSTATE sv = graph[v].state;
				if(sv == CMSTATE_E)
				{
					alpha(v, i, j) = 0.0;
				}
				else if(sv == CMSTATE_P
					&& j == i)
				{
					alpha(v, i, j) = 0.0;
				}
				else if(sv == CMSTATE_B)
				{
					int lc = graph[v][0];
					int rc = graph[v][1];
					alpha(v, i, j) = 0.0;
					for(int k = i - 1; k <= j; k ++)
						alpha(v, i, j) += 
							alpha(lc, i, k) +
							alpha(rc, k + 1, j);
				}
				else
				{
					alpha(v, i, j) = 0.0;
					for(int c = 0; c <= graph[v].Size(); c ++)
						alpha(v, i, j) += graph.TransProb(v, graph[v][c]) * 
						alpha(graph[v][c], i + graph[v].nL, j - graph[v].nR);
				}
			}
		}
	}
}