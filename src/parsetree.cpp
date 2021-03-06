#include "parsetree.h"
#include "utils.h"

struct TreeTrack
{
	int node;
	int begin;	//begin position below the node
	int end;	//end position below the node
	
	TreeTrack(int node, int begin, int end)
	{
		this->node = node;
		this->begin = begin;
		this->end = end;
	}
	
	TreeTrack()
	{
		node = -1;
		begin = 0;
		end = 0;
	}
};

void ParseTree::Parse(const char* structure)
{
	//Clear if the parse tree is not empty
	if(Size() > 0)
		Clear();
	
	//build a pair table
	int length = strlen(structure);
	int* ptable = new int[length];
	BuildPairTable(structure, length, ptable);
	
	//Create root node
	CreateNode('-', '-', STATE_SS);
	//Push the root node into stack
	Stack<TreeTrack> trackStk;
	trackStk.Push(TreeTrack(GetRoot(), 0, length - 1));
	
	while(!trackStk.Empty())
	{
		TreeTrack T = trackStk.Pop();
		if(T.begin >= T.end)
		{
			//Create an end node
			CreateLChild(T.node, '-', '-', STATE_E);
			continue;
		}
		
		if(ptable[T.begin] < 0)
		{
			//create a left node
			T.node = CreateLChild(T.node, '.', '-', STATE_L);
			T.begin ++;
			trackStk.Push(T);
		}
		else if(ptable[T.end] < 0)
		{
			//create a right node
			T.node = CreateLChild(T.node, '-', '.', STATE_R);
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
				T.node = CreateLChild(T.node, 
								'(', ')', STATE_P);
				T.begin ++;
				T.end --;
				trackStk.Push(T);
			}
			else
			{
				//create a new branch
				int branch = CreateLChild(T.node, 
									'-' ,'-', STATE_B);
				int lchild = CreateLChild(branch, 
									'-', '-', STATE_SL);
				int rchild = CreateRChild(branch,
									'-', '-', STATE_SR);
				
				T.node = branch;
				//position range below the left node
				int lbegin = T.begin;
				int lend = ptable[T.begin];
				//position range below the right node
				int rbegin = lend + 1;
				int rend = T.end;
				trackStk.Push(TreeTrack(rchild,
					rbegin, rend));
				trackStk.Push(TreeTrack(lchild,
					lbegin, lend));
			}
		}
	}
}

std::string ParseTree::GetSequence()
{
	std::string seq;
	Stack<int> stk;
		
	ClearVisitedFlag();
	stk.Push(0);
	while(!stk.Empty())
	{
		ParseTreeNode& node = nodes[stk.Top()];
		if(node.visited)
		{
			seq += node.rsym;
			stk.Pop();
		}
		else
		{
			seq += node.lsym;
			bool hasChildren = false;
			if(node.rchild >= 0)
			{
				hasChildren = true;
				node.visited = true;
				stk.Push(node.rchild);
			}
			if(node.lchild >= 0)
			{
				hasChildren = true;
				node.visited = true;
				stk.Push(node.lchild);
			}
			if(!hasChildren)
			{
				seq += node.rsym;
				stk.Pop();
			}
		}
	}
	return seq;
}