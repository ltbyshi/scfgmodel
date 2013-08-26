#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <vector>
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
	Vector<SYMBOL> Sample();
	//optimal sequence (most probable)
	Vector<SYMBOL> Optimal();
	//suboptimal sampling
	Vector<SYMBOL> Suboptimal();
private:
	void InitModel(int nStates);

	void Inside(const vector<SYMBOL>& seq);
	void Outside(const vector<SYMBOL& seq);
	void EMIter();
private:
	int nStates;
	ParseTree tree;
	CMGraph graph;
	//pair table
	std::vector<int> ptable; 
	std::vector<STATE> states;
	//e(v, xi, xj): emission probability matrix
	Matrix3D<PRECISION> em;	
	//t(v, y): transision probability matrix
	Matrix2D<PRECISION> tm; 
	//a(v, i, j): summed probability of all parse subtrees rooted at 
	//state v for the subsequence [i,...,j]
	Matrix3D<PRECISION> alpha;
	//b(v, h, j): summed probability of all parse subtrees rooted at
	//state v for the complete sequence excluding the subsequence
	//[i,...,j]
	Matrix3D<PRECISION> beta;
};
#endif
