#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <vector>
#include <list>
#include "matrix.h"
#include "parsetree.h"
#include "cmgraph.h"
#include "symbol.h"

class SCFGModel
{
public:
	SCFGModel();
	~SCFGModel();
	//Build a model from RNA secondary structure
	//The structure is in (.) format
	void Build(const char* structure);
	
	PRECISION EmitProb(STATE state, SYMBOL left, SYMBOL right);
	PRECISION TransProb(STATE curState, STATE nextState);
	//sample a sequence
	std::vector<SYMBOL> Sample();
	//optimal sequence (most probable)
	std::vector<SYMBOL> Optimal();
	//suboptimal sampling
	std::vector<SYMBOL> Suboptimal();
	//Add a training sequence
	void AddSequence(const std::vector<SYMBOL>& seq);
	//Clear all training sequences
	void ClearSequences();
private:
	void InitModel(int nStates);
	//Inside algorithm for one sequence
	void Inside(const std::vector<SYMBOL>& seq);
	//Outside algorithm for one sequence
	void Outside(const std::vector<SYMBOL>& seq);
	//Calculate expected counts for each state transition and 
	//symbol emission
	void Expectation();
	//Estimate model parameters from expected counts
	void Maximization();
	void EMIter();
private:
	int nStates;
	ParseTree tree;
	CMGraph graph;
	//pair table
	std::vector<int> ptable; 
	//training sequences
	std::list<std::vector<SYMBOL> > sequences;
	std::vector<STATE> states;
	//a(v, i, j): summed probability of all parse subtrees rooted at 
	//state v for the subsequence [i,...,j]
	Matrix3D<PRECISION> alpha;
	//b(v, h, j): summed probability of all parse subtrees rooted at
	//state v for the complete sequence excluding the subsequence
	//[i,...,j]
	Matrix3D<PRECISION> beta;
};
#endif
