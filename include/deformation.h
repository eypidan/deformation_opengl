#ifndef DEFORMATION_H
#define DEFORMATION_H

using namespace Eigen;
struct controlMove {
	float x =0;
	float y =0;
	float z =0;

};

void dataInitial(Mesh* mesh);

//convert a vertex Vectir to Eigen's Matrix
void convertToEigenMatrix(vector<Vertex> a, VectorXf& V);
void convertFromEigenMatrix(vector<Vertex> &a, VectorXf V);
void converToROIMatrix(Mesh* mesh, VectorXf& V);
//void calcAdjancyMatrix(Mesh* mesh, SparseMatrix<float>& adj);
void deform(Mesh* mesh);
void calcLaplacianCoff(Mesh* mesh, vector<Triplet<float>>& lapCoff); //only cal Roi region
vector<Triplet<float>> calcEnergyCoff(Mesh* mesh, vector<Triplet<float>>& lapCoff);
#endif