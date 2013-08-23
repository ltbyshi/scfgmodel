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
	//build a parse tree from RNA structure
	tree.Parse(structure);
	//fill the states from the parse tree
	nStates = 0;
	for(int i = 0; i < tree.Size(); i ++)
	{
		STATE state = tree[i].state;
		nStates ++;
		states.push_back(tree[i].state);
		switch(state)
		{
			case STATE_P:
			case STATE_L:
			case STATE_R:
				nState ++;
		}
	}
	em.Resize(nStates, NUMSYMBOLS, NUMSYMBOLS);
	tm.Resize(nStates, nStates);
	//insert deletion states from the parse tree
	for(int i = 0; i < tree.Size(); i ++)
	{
		STATE state = tree[i].state;
		switch(state)
		{
			case STATE_P:
			case STATE_L:
			case STATE_R:
				states.push_back(STATE_D);
		}
	}
	
}