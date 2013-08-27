#ifndef __SYMBOL_H__
#define __SYMBOL_H__
/*
 * symbols used in the CFGs
 */

enum SYMBOL {
	SYMBOL_A = 0,
	SYMBOL_U,
	SYMBOL_G,
	SYMBOL_C,
	SYMBOL_EPS,
	SYMBOL_ANY
};

const char* const SymbolName[] = {
	"A", "U", "G", "C", "e", "?"
};

const SYMBOL ALLSYMBOLS[] = {
	SYMBOL_A,
	SYMBOL_U,
	SYMBOL_G,
	SYMBOL_C
};
const int NUMSYMBOLS = sizeof(ALLSYMBOLS) / sizeof(SYMBOL);

// SCFG states
enum STATE {
	STATE_P = 0,
	STATE_L,
	STATE_R,
	STATE_B,
	STATE_E,
	STATE_SS,
	STATE_SL,
	STATE_SR,
	STATE_ANY
};

const char* const StateName[] = {
	"P", "L", "R", "B", "E", "SS", "SL", "SR", "?"
};

const STATE ALLSTATES[] = {
	STATE_P,
	STATE_L,
	STATE_R,
	STATE_SS,
	STATE_SL,
	STATE_SR,
	STATE_B,
	STATE_E
};
const int NUMSTATES = sizeof(ALLSTATES) / sizeof(STATE);


// states in covariance models
enum CMSTATE {
	CMSTATE_S = 0,
	CMSTATE_ML,
	CMSTATE_MR,
	CMSTATE_MP,
	CMSTATE_IL,
	CMSTATE_IR,
	CMSTATE_D,
	CMSTATE_E,
	CMSTATE_B,
	CMSTATE_ANY
};

const char* const CMStateName[] = {
	"S", "ML", "MR", "MP", "IL", "IR", "D", "E", "B", "?"
};

const CMSTATE ALLCMSTATES[] = {
	CMSTATE_S,
	CMSTATE_ML,
	CMSTATE_MR,
	CMSTATE_MP,
	CMSTATE_IL,
	CMSTATE_IR,
	CMSTATE_D,
	CMSTATE_E,
	CMSTATE_B
};
const int NUMCMSTATES = sizeof(ALLCMSTATES) / sizeof(CMSTATE);

typedef double PRECISION;

#endif