#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

typedef unsigned int SYMBOL;

struct Production {
	int nStates;
	SYMBOL nextStates[2];
	int emitLeft;
	int emitRight;
};

typedef double PRECISION;

/* terminal symbols */
#define SYMBOL_A	1
#define SYMBOL_U	2
#define SYMBOL_G	3
#define SYMBOL_C	4
#define SYMBOL_EPS	5

typedef int STATE;
/* nonspecific states (nonterminals) */
#define STATE_ANY	0
/* specific states */
#define STATE_P		1
#define STATE_L		2
#define STATE_R		3
#define STATE_D		4
#define STATE_S		5
#define STATE_B		6
#define STATE_E		7
/* productions */
const Production PROD_P = {1, {STATE_ANY}, 1, 1};
const Production PROD_L = {1, {STATE_ANY}, 1, 0};
const Production PROD_R = {1, {STATE_ANY}, 0, 1};
const Production PROD_D = {1, {STATE_ANY}, 0, 0};
const Production PROD_S = {1, {STATE_ANY}, 0, 0};
const Production PROD_B = {2, {STATE_ANY, STATE_ANY}, 0, 0};
const Production PROD_E = {0, {}, 0, 0};
/* SCFG */
struct SCFGModel {
	int nStates;
	STATE* states;
	PRECISION** emitTable;
	PRECISION** transTable;
	int curState;
};

SCFGModel* CreateSCFGModel(int nStates);
void FreeSCFGModel(SCFGModel* model);
PRECISION GetEmitProb(SCFGModel* model, STATE state, 
		SYMBOL left, SYMBOL right);
PRECISION GetTransProb(SCFGModel* model, STATE curState,
		STATE nextState);
void SampleModel(SCFGModel* model, SYMBOL* seq, int length);
void Likelihood(SCFGModel* model, SYMBOL* seq, int length);
void CalcAlpha(SCFGModel* model, SYMBOL*seq, int length);
void CalcBeta(SCFGModel* model, SYMBOL* seq, int length);
#endif
