#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstring>

using namespace std;

#include "queue.h"
#include "tree.h"

void die(const char* message)
{
	cerr << "Error: " << message << endl;
	exit(1);
}

#define CASERETURN(value, name) \
	case value: \
		return #name;

SYMBOL allSymbols[8] = {
	SYMBOL_A,
	SYMBOL_U,
	SYMBOL_G,
	SYMBOL_C,
	SYMBOL_LP,
	SYMBOL_RP,
	SYMBOL_SS,
	SYMBOL_EPS
};

const char* SymbolName(SYMBOL symbol)
{
	switch(symbol)
	{
		CASERETURN(SYMBOL_A, A)
		CASERETURN(SYMBOL_U, U)
		CASERETURN(SYMBOL_G, G)
		CASERETURN(SYMBOL_C, C)
		CASERETURN(SYMBOL_LP, L)
		CASERETURN(SYMBOL_RP, R)
		CASERETURN(SYMBOL_SS, S)
		CASERETURN(SYMBOL_EPS, e)
		default:
			return "?";
	}
}

STATE allStates[7] = {
	STATE_P,
	STATE_L,
	STATE_R,
	STATE_D,
	STATE_S,
	STATE_B,
	STATE_E
};

const char* StateName(STATE state)
{
	switch(state)
	{
		CASERETURN(STATE_P, P)
		CASERETURN(STATE_L, L)
		CASERETURN(STATE_R, R)
		CASERETURN(STATE_D, D)
		CASERETURN(STATE_S, S)
		CASERETURN(STATE_B, B)
		CASERETURN(STATE_E, E)
		default:
			return "?";
	}
}

template <class T>
T Choose(const T* choices, int n)
{
	return choices[rand() % n];
}

#if 1
struct TreeLevel
{
	ParseTreeNode* node;
	int level;

	TreeLevel()
	{
		this->node = NULL;
		this->level = 0;
	}

	TreeLevel(ParseTreeNode* node, int level)
	{
		this->node = node;
		this->level = level;
	}
};

struct NodeWriter
{
	ostream& os;
	
	NodeWriter(ostream& os)
		:os(os)
	{
	}
	
	void WriteEdge(ParseTreeNode* parent, ParseTreeNode* child)
	{
		os << "\tNode_" << parent << " [label=\"<lc> "
			<< SymbolName(parent->lsym) << "|<st> "
			<< StateName(parent->state) << "|<rc> "
			<< SymbolName(parent->rsym) << "\"];" << endl;
		os << "\tNode_" << child << " [label=\"<lc> "
			<< SymbolName(child->lsym) << "|<st> "
			<< StateName(child->state) << "|<rc> "
			<< SymbolName(child->rsym) << "\"];" << endl;
		os << "\tNode_" << parent << ":st"
			<< " -> Node_" << child << ":st"
			<< ";" << endl;
	}
	
	void operator()(ParseTreeNode* node)
	{
		if(node->lchild)
			WriteEdge(node, node->lchild);
		if(node->rchild)
			WriteEdge(node, node->rchild);
	}
};

void PlotParseTree(const char* filename, ParseTree& tree)
{
	ofstream fout(filename);
	if(!fout)
		die("Cannot open the file");

	fout << "digraph{" << endl;
	fout << "\tnode [shape=record];" << endl;
	tree.PreOrder(NodeWriter(fout));
	fout << "}";
	fout.close();
}

ParseTreeNode* RandomNode()
{
	ParseTreeNode* node = new ParseTreeNode;
	node->state = Choose(allStates, 8);
	node->lsym = Choose(allSymbols, 5);
	node->rsym = Choose(allSymbols, 5);
	
	return node;
}

void BuildPairTable(const char* structure, int length, int* table)
{
	Stack<int> S;
	for(int i = 0; i < length; i ++)
	{
		switch(structure[i])
		{
			case '(':
				S.Push(i);
				break;
			case '.':
				table[i] = -1;
				break;
			case ')':
				if(S.Empty())
					die("Brackets do not match");
				table[S.Top()] = i;
				table[i] = S.Pop();
				break;
			default:
				die("Junk character found in RNA structure");
		}
	}
}

struct TreeTrack
{
	ParseTreeNode* node;
	int begin;	//begin position below the node
	int end;	//end position below the node
	
	TreeTrack(ParseTreeNode* node, int begin, int end)
	{
		this->node = node;
		this->begin = begin;
		this->end = end;
	}
	
	TreeTrack()
	{
		node = NULL;
		begin = 0;
		end = 0;
	}
};

//build a tree from RNA secondary structure
ParseTree* BuildParseTree(const char* structure)
{
	int length = strlen(structure);
	int* ptable = new int[length];
	BuildPairTable(structure, length, ptable);
	
	ParseTree* tree = new ParseTree;
	ParseTreeNode* root = tree->GetRoot();
	
	Stack<TreeTrack> trackStk;
	trackStk.Push(TreeTrack(root, 0, length - 1));
	while(!trackStk.Empty())
	{
		TreeTrack T = trackStk.Pop();
		if(T.begin >= T.end)
		{
			//Create an end node
			T.node->lchild = new ParseTreeNode(SYMBOL_EPS,
				SYMBOL_EPS, STATE_E);
			continue;
		}
		
		if(ptable[T.begin] < 0)
		{
			//create a left node
			T.node->lchild = new ParseTreeNode(SYMBOL_SS,
				SYMBOL_EPS, STATE_L);
			T.node = T.node->lchild;
			T.begin ++;
			trackStk.Push(T);
		}
		else if(ptable[T.end] < 0)
		{
			//create a right node
			T.node->lchild = new ParseTreeNode(SYMBOL_EPS,
				SYMBOL_SS, STATE_R);
			T.node = T.node->lchild;
			T.end --;
			trackStk.Push(T);
		}
		else
		{
			if(ptable[T.begin] == T.end &&
				ptable[T.end] == T.begin)
			{
				//end pairs
				//create a pair node
				T.node->lchild = new ParseTreeNode(SYMBOL_LP,
					SYMBOL_RP, STATE_P);
				T.node = T.node->lchild;
				T.begin ++;
				T.end --;
				trackStk.Push(T);
			}
			else
			{
				//create a new branch
				ParseTreeNode* branch = new ParseTreeNode(SYMBOL_EPS,
					SYMBOL_EPS, STATE_B);
				branch->lchild = new ParseTreeNode(SYMBOL_EPS,
					SYMBOL_EPS, STATE_S);
				branch->rchild = new ParseTreeNode(SYMBOL_EPS,
					SYMBOL_EPS, STATE_S);
				T.node->lchild = branch;
				//position range below the left node
				int lbegin = T.begin;
				int lend = ptable[T.begin];
				//position range below the right node
				int rbegin = lend + 1;
				int rend = T.end;
				trackStk.Push(TreeTrack(branch->rchild,
					rbegin, rend));
				trackStk.Push(TreeTrack(branch->lchild,
					lbegin, lend));
			}
		}
	}
	
	delete ptable;
	return tree;
}

ParseTree* RandomParseTree(int nNodes)
{
	ParseTree* tree = new ParseTree;
	ParseTreeNode* root = tree->GetRoot();
	ParseTreeNode* curNode = root;

	Queue<ParseTreeNode*> Q;
	Q.Push(root);
	for(int i = 0; i < nNodes; i ++)
	{
		ParseTreeNode* node = Q.Pop();
		node->lchild = RandomNode();
		node->rchild = RandomNode();
		Q.Push(node->lchild);
		Q.Push(node->rchild);
	}
	
	return tree;
}

void Test_ParseTree()
{
	//ParseTree* tree = RandomParseTree(10);
	ParseTree* tree = BuildParseTree("..(((....).)).((.(...)))");
	
	PlotParseTree("tree.dot", *tree);
	system("dot -Tpng -o tree.png tree.dot");

	tree->ClearVisitedFlag();
	Sequence<SYMBOL> seq = tree->GetSequence();

	cout << "sequence: ";
	for(int i = 0; i < seq.Size(); i ++)
		cout << SymbolName(seq[i]) << " ";
	cout << endl;
}
#endif

#if 0
void Test_Queue()
{
	Queue<int> queue(5);
	cout << "Pushed: ";
	for(int i = 0; i < 50; i ++)
	{
		queue.Push(i);
		cout << i << " ";
	}
	cout << endl;

	cout << "Poped: ";
	for(int i = 0; i < 51; i ++)
	{
		int e = queue.Pop();
		cout << e << " ";
	}
	cout << endl;
}
#endif

int main()
{
	srand((unsigned int)time(0));
	//Test_Queue();
	Test_ParseTree();
	return 0;
}
