#ifndef __PROBMAT_H__
#define __PROBMAT_H__

typedef double PRECISION;

typedef struct ProbMat1D_s{
	int size;
	PRECISION* data;
}ProbMat1D;

typedef struct ProbMat2D_s{
	int size;
	int size1;
	int size2;
	PRECISION* data;
}ProbMat2D;

typedef struct ProbMat3D_s{
	int size;
	int size1;
	int size2;
	int size23; /* size1 * size2 */
	int size3;
	PRECISION* data;
}ProbMat3D;

ProbMat1D* CreateProbMat1D(int size);
void FreeProbMat1D(ProbMat1D* pm);
ProbMat2D* CreateProbMat2D(int size1, int size2);
void FreeProbMat2D(ProbMat2D* pm);
ProbMat3D* CreateProbMat3D(int size1, int size2, int size3);
void FreeProbMat3D(ProbMat3D* pm);

#define ProbMatGet1D(pm, i) (pm->data[i])
#define ProbMatGet2D(pm, i, j) (pm->data[i*pm->size2 + j])
#define ProbMatGet3D(pm, i, j, k) (pm->data[i*pm->size23 + j*pm->size3 + k])

#endif
