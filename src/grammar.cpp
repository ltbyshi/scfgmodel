#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <signal.h>
using namespace std;

#include "grammar.h"
#include "stack.h"
#include "utils.h"

//#define ENABLE_PRIOR

SCFGModel::SCFGModel()
{
	maxIter = 1;
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

void SCFGModel::AddSequence(const vector<SYMBOL>& seq)
{
	sequences.push_back(seq);
}

void SCFGModel::ClearSequences()
{
	sequences.clear();
}

void SCFGModel::Inside(const vector<SYMBOL>& seq)
{
	int L = seq.size();	//sequence length
	int M = graph.Size();	//number of states
	
	alpha.Clear();
	alpha.Resize(M, L + 2, L + 1);
	//Initialization
	for(int j = 0; j <= L; j ++)
	{
		for(int v = M - 1; v >= 0; v --)
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
						graph[v].tp[c]
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
		for(int i = j; i >= 1; i --)
		{
			for(int v = M - 1; v >= 0; v --)
			{
				CMSTATE sv = graph[v].state;
				if(sv == CMSTATE_E)
				{
					alpha(v, i, j) = 0.0;
				}
				else if(sv == CMSTATE_MP
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
							alpha(lc, i, k) *
							alpha(rc, k + 1, j);
				}
				else
				{
					alpha(v, i, j) = 0.0;
					for(int c = 0; c < graph[v].Size(); c ++)
						alpha(v, i, j) += graph[v].tp[c] * 
						alpha(graph[v][c], i + graph[v].nL, j - graph[v].nR);
					int xi = seq[i - 1];
					int xj = seq[j - 1];
					alpha(v, i, j) *= graph[v].ep[xi][xj];
				}
			}
		}
	}
}

void SCFGModel::Outside(const vector<SYMBOL>& seq)
{
	int L = seq.size();
	int M = graph.Size();
	
	beta.Clear();
	beta.Resize(M, L + 2, L + 1);
	//Initialization
	beta(0, 1, L) = 1.0;
	//Recursion
	for(int i = 1; i <= L + 1; i ++)
	{
		for(int j = L; j >= i - 1; j --)
		{
			for(int v = 1; v <= M - 1; v ++)
			{
				CMSTATE sv = graph[v].state;
				if(sv == CMSTATE_S)
				{
					//S node has only one parent
					int p = graph[v].parents[0];
					//B node has two children
					int lc = graph[p][0];
					int rc = graph[p][1];
					
					beta(v, i, j) = 0.0;
					if(lc == sv)
					{
						//v is left child
						for(int k = j; k <= L; k ++)
							beta(v, i, j) += beta(p, i, k)*alpha(rc, j + 1, k);
					}
					else if(rc == sv)
					{
						//v is right child
						for(int k = 1; k <= i; k ++)
							beta(v, i, j) += beta(p, k, j)*alpha(lc, k, i - 1);
					}
					else
					{
						//invalid state
						Die("Invalid state found: %d", v);
					}
				}
				else
				{
					beta(v, i, j) = 0.0;
					//MP, ML, MR, D, B, E, IL, IR
					for(size_t y = 0; y < graph[v].parents.size(); y ++)
					{
						int p = graph[v].parents[y];
						if((i - graph[p].nL - 1 < 0)
							|| (i - graph[p].nL - 1 >= L)
							|| (j + graph[p].nR - 1 < 0)
							|| (j + graph[p].nR - 1 >= L))
						{	//avoid indexing out of range
							//cout << "beta(" << v << ", " << i << ", " << j << ") is out of range" << endl;
							beta(v, i, j) += 0.0;
						}
						else
						{
							int xi = seq[i - graph[p].nL - 1];
							int xj = seq[j + graph[p].nR - 1];
							//Find index of node v in the children of node p
							int vi = 0;
							for(vi = 0; vi < graph[p].Size(); vi ++)
							{
								if(graph[p][vi] == v)
									break;
							}
							
							beta(v, i, j) += graph[p].ep[xi][xj] * graph[p].tp[vi] * beta(p, i - graph[p].nL, j + graph[p].nR);
							
							if(isnan(fabs(beta(v, i, j))))
							{
								cout << "beta(" << v << ", " << i << ", " << j << ") = " << beta(v, i, j) << endl;
								raise(SIGTRAP);
							}
						}
					}
				}
			}
		}
	}
}

void SCFGModel::Expectation()
{
	//Initialize expected counts to zero
	for(int v = 0; v < graph.Size(); v ++)
	{
		//Total number of transitions
		graph[v].Ttot = 0.0;
		//Total number of emissions
		graph[v].Etot = 0.0;
		//transition count
		for(int c = 0; c < graph[v].Size(); c ++)
			graph[v].tc[c] = 0.0;
		//emission count
		for(int i = 0; i < NUMSYMBOLS; i ++)
			for(int j = 0; j < NUMSYMBOLS; j ++)
				graph[v].ec[i][j] = 0.0;
	}
	//Calculate expected counts for each sequence
	int M = graph.Size();	//number of states
	for(list<vector<SYMBOL> >::iterator iter = sequences.begin(); iter != sequences.end(); ++ iter)
	{
		vector<SYMBOL>& seq = *iter;
		int L = seq.size();	//length of the sequence
		//Run inside-outside algorithm
		Inside(seq);
		Outside(seq);
		
		//probability of the appearance of the sequence
		PRECISION Px = alpha(0, 1, L);
		if(Px < 1.0e-100 && Px > -1.0e-100)
			continue;
		cout << "P(x) = " << Px << endl;
		
		/*
		alpha.DumpText("alpha.mat");
		beta.DumpText("beta.mat");
		exit(-1);
		*/
		
		//Calculate total number of transitions
		for(int v = 0; v < M; v ++)
		{
			PRECISION Ttot = 0.0;
			for(int i = 1; i <= L + 1; i ++)
				for(int j = i - 1; j <= L; j ++)
					Ttot += alpha(v, i, j)*beta(v, i, j);
			//Add to sum over all sequences
			graph[v].Ttot += Ttot / Px;
		}
		//Calculate number of transitions to next state
		for(int v = 0; v < M; v ++)
		{
			for(int c = 0; c < graph[v].Size(); c ++)
			{
				PRECISION tc = 0.0;
				for(int i = 1; i <= L; i ++)
				{
					for(int j = i - 1; j <= L; j ++)
					{
						int xi = seq[i - 1];
						int xj = seq[j - 1];
						tc += beta(v, i, j)
							* graph[v].ep[xi][xj]
							* graph[v].tp[c]
							* alpha(graph[v][c], i + graph[v].nL, j - graph[v].nR);
					}
				}
				//Add to sum over all sequences
				graph[v].tc[c] += tc / Px;
			}
		}
		//Calculate number of emissions
		for(int v = 0; v < graph.Size(); v ++)
		{
			CMSTATE sv = graph[v].state;
			//Expected number of emissions from this state
			PRECISION Etot = 0.0;
			PRECISION ec[NUMSYMBOLS][NUMSYMBOLS] = {{0.0}};
			for(int i = 1; i <= L; i ++)
			{
				for(int j = 1; j <= L; j ++)
				{
					//symbols at both end of [i...j]
					SYMBOL xi = seq[i - 1];
					SYMBOL xj = seq[j - 1];
					//Inside-outside variable
					PRECISION ab = alpha(v, i, j)*beta(v, i, j);
					Etot += ab;
					if(sv == CMSTATE_MP)
					{
						//Emit a pair
						for(int a = 0; a < NUMSYMBOLS; a ++)
						{
							for(int b = 0; b < NUMSYMBOLS; b ++)
							{
								if(xi == a && xj == b)
									ec[a][b] += ab;
							}
						}
					}
					else if(sv == CMSTATE_ML
						|| sv == CMSTATE_IL)
					{
						//Emit a left symbol
						for(int a = 0; a < NUMSYMBOLS; a ++)
						{
							if(xi == a)
								ec[a][0] += ab;
						}
					}
					else if(sv == CMSTATE_MR
						|| sv == CMSTATE_IR)
					{
						//Emit a right symbol
						for(int a = 0; a < NUMSYMBOLS; a ++)
						{
							if(xj == a)
								ec[a][0] += ab;
						}
					}
					else
					{
						//Emit no symbols
					}
				}
			}
			//Add to sum over all sequences
			graph[v].Etot += Etot / Px;
			for(int a = 0; a < NUMSYMBOLS; a ++)
				for(int b = 0; b < NUMSYMBOLS; b ++)
					graph[v].ec[a][b] += ec[a][b] / Px;
		}
	}
}

void SCFGModel::Maximization()
{
	//Estimate transition probability
	for(int v = 0; v < graph.Size(); v ++)
	{
		//B states need not be estimated
		if(graph[v].state != CMSTATE_B)
		{
#ifdef ENABLE_PRIOR
			//Prior transition probability
			PRECISION prior;
			if(graph[v].Size() > 0)
				prior = 1.0 / graph[v].Size();
			else
				//E state
				prior = 1.0;
			for(int c = 0; c < graph[v].Size(); v ++)
				graph[v].tp[c] = (graph[v].tc[c] + prior) / (graph[v].Ttot + 1.0);
#else
			for(int c = 0; c < graph[v].Size(); c ++)
				graph[v].tp[c] = graph[v].tc[c] / graph[v].Ttot;
#endif
		}
	}
	//Estimate emission probability
	for(int v = 0; v < graph.Size(); v ++)
	{
		CMSTATE sv = graph[v].state;
		if(sv == CMSTATE_MP)
		{
			//Emit a pair
			for(int a = 0; a < NUMSYMBOLS; a ++)
				for(int b = 0; b < NUMSYMBOLS; b ++)
#ifdef ENABLE_PRIOR
					graph[v].ep[a][b] = (graph[v].ec[a][b] + 0.25) / (graph[v].Etot + 1.0);
#else
					graph[v].ep[a][b] = graph[v].ec[a][b] / graph[v].Etot;
#endif
		}
		else if(sv == CMSTATE_IL
			|| sv == CMSTATE_ML
			|| sv == CMSTATE_IR
			|| sv == CMSTATE_MR)
		{
			//Emit a single symbol
			for(int a = 0; a < NUMSYMBOLS; a ++)
#ifdef ENABLE_PRIOR
				graph[v].ep[a][0] = (graph[v].ec[a][0] + 0.25) / (graph[v].Etot + 1.0);
#else
				graph[v].ep[a][0] = graph[v].ec[a][0] / graph[v].Etot;
#endif
		}
		else
		{
			//Emit no symbols
		}
	}
}

void SCFGModel::Train()
{
	for(int i = 0; i < maxIter; i ++)
	{
		Expectation();
		Maximization();
	}
}

void SCFGModel::SetMaxIter(int n)
{
	maxIter = n;
}

CMGraph& SCFGModel::GetCMGraph()
{
	return graph;
}