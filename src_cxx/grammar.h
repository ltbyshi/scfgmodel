#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include "matrix.h"

enum SYMBOL {
	SYMBOL_A,
	SYMBOL_U,
	SYMBOL_G,
	SYMBOL_C,
	SYMBOL_EPS
};

enum STATE {
	STATE_ANY,
	STATE_P,
	STATE_L,
	STATE_R,
	STATE_D,
	STATE_S,
	STATE_B,
	STATE_E
}

typedef PRECISION double;

class SCFGModel
{
public:
	SCFGModel(int nStates);
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

	void Inside(const Vector<SYMBOL>& seq);
	void Outside(const Vector<SYMBOL& seq);
	void EMIter();
private:
	int nStates;
	Vector<STATE> states;
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
