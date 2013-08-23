#ifndef __SYMBOL_H__
#define __SYMBOL_H__
/*
 * symbols used in the CFGs
 */

enum SYMBOL {
	SYMBOL_A,
	SYMBOL_U,
	SYMBOL_G,
	SYMBOL_C,
	SYMBOL_LP,
	SYMBOL_RP,
	SYMBOL_SS,
	SYMBOL_EPS
};

const SYMBOL ALLSYMBOLS[] = {
	SYMBOL_A,
	SYMBOL_U,
	SYMBOL_G,
	SYMBOL_C,
	SYMBOL_LP,
	SYMBOL_RP,
	SYMBOL_SS,
	SYMBOL_EPS
};
const int NUMSYMBOLS = sizeof(ALLSYMBOLS) / sizeof(SYMBOL);

// SCFG states
enum STATE {
	STATE_P,
	STATE_L,
	STATE_R,
	STATE_D,
	STATE_S,
	STATE_B,
	STATE_E,
	STATE_ANY
};

const STATE ALLSTATES[] = {
	STATE_P,
	STATE_L,
	STATE_R,
	STATE_D,
	STATE_S,
	STATE_B,
	STATE_E,
	STATE_ANY
};
const int NUMSTATES = sizeof(ALLSTATES) / sizeof(STATE);

// states in covariance models
enum CMSTATE {
	CMSTATE_S,
	CMSTATE_ML,
	CMSTATE_MR,
	CMSTATE_MP,
	CMSTATE_IL,
	CMSTATE_IR,
	CMSTATE_D,
	CMSTATE_E
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
	CMSTATE_ANY
};
const int NUMCMSTATES = sizeof(ALLCMSTATES) / sizeof(CMSTATE);
#define CASERETURN(value, name) \
	case value: \
		return name;

inline const char* SymbolName(SYMBOL symbol)
{
	switch(symbol)
	{
		CASERETURN(SYMBOL_A, "A")
		CASERETURN(SYMBOL_U, "U")
		CASERETURN(SYMBOL_G, "G")
		CASERETURN(SYMBOL_C, "C")
		CASERETURN(SYMBOL_LP, "(")
		CASERETURN(SYMBOL_RP, ")")
		CASERETURN(SYMBOL_SS, ".")
		CASERETURN(SYMBOL_EPS, "e")
		default:
			return "?";
	}
}

inline const char* StateName(STATE state)
{
	switch(state)
	{
		CASERETURN(STATE_P, "P")
		CASERETURN(STATE_L, "L")
		CASERETURN(STATE_R, "R")
		CASERETURN(STATE_D, "D")
		CASERETURN(STATE_S, "S")
		CASERETURN(STATE_B, "B")
		CASERETURN(STATE_E, "E")
		CASERETURN(STATE_ANY, "?")
		default:
			return "?";
	}
}

inline const char* CMStateName(CMSTATE state)
{
	switch(state)
	{
		CASERETURN(CMSTATE_S, "S")
		CASERETURN(CMSTATE_ML, "ML")
		CASERETURN(CMSTATE_MR, "MR")
		CASERETURN(CMSTATE_MP, "MP")
		CASERETURN(CMSTATE_IL, "IL")
		CASERETURN(CMSTATE_IR, "IR")
		CASERETURN(CMSTATE_D, "D")
		CASERETURN(CMSTATE_E, "E")
		CASERETURN(CMSTATE_ANY, "?")
		default:
			return "?";
	}
}

#undef CASERETURN

#endif