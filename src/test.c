#include <stdio.h>
#include "probmat.h"

void Test_probmat()
{
	ProbMat1D* pm1 = CreateProbMat1D(1000);
	ProbMatGet1D(pm1, 3) = 0.0;
	FreeProbMat1D(pm1);

	ProbMat2D* pm2 = CreateProbMat2D(100, 100);
	ProbMatGet2D(pm2, 5, 3) = 3.0;
	FreeProbMat2D(pm2);
}
int main(int argc, char** argv)
{
	Test_probmat();

	return 0;
}
