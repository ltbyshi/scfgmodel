#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "grammar.h"
#include "utils.h"

using namespace std;

//argv[1]: seed structure
//argv[2]: training sequences
int main(int argc, char** argv)
{
	/*
	if(argc != 3)
		Die("Usage: %s <seedfile> <seqfile>", argv[0]);
	const char* seedfile = argv[1];
	const char* seqfile = argv[2];
	*/
	const char* seedfile = "../data/seed.txt";
	const char* seqfile = "../data/seqs.txt";
	
	SCFGModel model;
	//Build SCFG model from seed structure
	ifstream fin;
	fin.open(seedfile);
	if(!fin)
		Die("Cannot open the seed file %s", argv[1]);
	string seed;
	fin >> seed;
	fin.close();
	
	model.Build(seed.c_str());
	//Add training sequences
	fin.open(seqfile);
	if(!fin)
		Die("Cannot open the seq file %s", argv[2]);
	while(!fin.eof())
	{
		string seq;
		fin >> seq;
		if(seq.size() > 0)
			model.AddSequence(StrToSeq(seq.c_str()));
	}
	fin.close();
	//Plot the CM graph
	PlotCMGraph("graph_initial.dot", model.GetCMGraph());
	model.GetCMGraph().Dump("graph_initial.txt");
	system("dot -Tpdf -o graph_initial.pdf graph_initial.dot");
	//Train the model
	model.SetMaxIter(5);
	model.Train();
	//Plot the CM graph
	PlotCMGraph("graph_trained.dot", model.GetCMGraph());
	model.GetCMGraph().Dump("graph_trained.txt");
	system("dot -Tpdf -o graph_trained.pdf graph_trained.dot");
	
	return 0;
}