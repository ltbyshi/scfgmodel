#include <stdlib.h>
#include "probmat.h"

ProbMat1D* CreateProbMat1D(int size)
{
	ProbMat1D* pm;
	
	pm = (ProbMat1D*)malloc(sizeof(ProbMat1D));
	pm->size = size;
	pm->data = (PRECISION*)malloc(sizeof(PRECISION) * pm->size);
	return pm;
}

void FreeProbMat1D(ProbMat1D* pm)
{
	free(pm->data);
	free(pm);
}

ProbMat2D* CreateProbMat2D(int size1, int size2)
{
	ProbMat2D* pm;
	
	pm = (ProbMat2D*)malloc(sizeof(ProbMat2D));
	pm->size1 = size1;
	pm->size2 = size2;
	pm->size = size1 * size2;
	pm->data = (PRECISION*)malloc(sizeof(PRECISION) * pm->size);
	return pm;
}

void FreeProbMat2D(ProbMat2D* pm)
{
	free(pm->data);
	free(pm);
}

ProbMat3D* CreateProbMat3D(int size1, int size2, int size3)
{
	ProbMat3D* pm;
	
	pm = (ProbMat3D*)malloc(sizeof(ProbMat3D));
	pm->size1 = size1;
	pm->size2 = size2;
	pm->size3 = size3;
	pm->size23 = size2 * size3;
	pm->data = (PRECISION*)malloc(sizeof(PRECISION) * pm->size);
	return pm;
}

void FreeProbMat3D(ProbMat3D* pm)
{
	free(pm->data);
	free(pm);
}
	
