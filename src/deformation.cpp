#include "pch.h"

using namespace Eigen;


static vector<Triplet<float>> originLaplacian,energe;
static SparseMatrix<float> LCoffMatrix;
static MatrixXf VertexMatrix, L, solution;
static SimplicialLLT<SparseMatrix<float>> solver;


unsigned int selectedPoint;


void dataInitial(Mesh* mesh) {


	convertToEigenMatrix(mesh->vertices, VertexMatrix);
	//calcAdjancyMatrix(mesh, adjancyMatrix);
 	calcLaplacianCoff(mesh, originLaplacian);
	energe = originLaplacian;
	for (int i = 0; i < mesh->formalIndice.size(); i++) {
		int index = mesh->formalIndice[i];
		energe.push_back(Triplet<float>(index, index, 1.0));
	}

	LCoffMatrix.resize(mesh->vertices.size(), mesh->vertices.size());
	LCoffMatrix.reserve(10* mesh->vertices.size());
	LCoffMatrix.setFromTriplets(energe.begin(), energe.end());

	L = LCoffMatrix* VertexMatrix;

	auto start = chrono::high_resolution_clock::now();

	solver.compute(LCoffMatrix.transpose()* LCoffMatrix);

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	cout << "Factorizating engery matrix use : " << duration.count() / (double)1000000.0 << " s" << endl;
}


void deform(Mesh* mesh) {
	auto start = chrono::high_resolution_clock::now();

	selectedPoint = bunnyControlPoints[0];
	for (int i = 0; i < mesh->handleIndice.size(); i++) {
		int index = mesh->handleIndice[i];
		mesh->vertices[index].Position[0] += -0.01;
		mesh->vertices[index].Position[1] += 0.01;
		mesh->vertices[index].Position[2] += 0.01;
	}
	
	convertToEigenMatrix(mesh->vertices, VertexMatrix);
	L = LCoffMatrix * VertexMatrix;

	solution = solver.solve(LCoffMatrix.transpose() * L);
	convertFromEigenMatrix(mesh->vertices, solution);
	
	cout << "DEBUG:: change: " << (solution - VertexMatrix).norm() << endl;

	mesh->updateVertex();

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	cout << "Deform use : " << duration.count() / 1000000.0 << " s" << endl;
}

void calcLaplacianCoff(Mesh* mesh, vector<Triplet<float>>& lapCoff) {
	// this function use uniform laplacian
	for (int i = 0; i < mesh->ROIindice.size(); i++) {
		int index = mesh->ROIindice[i];
		unsigned int degree = mesh->adjMatrix[index].size();
		float weight = -1.0 / degree;
		for (int j = 0; j < degree; j++) {
			int j_index = mesh->adjMatrix[index][j];
			lapCoff.push_back(Triplet<float>(index, j_index,weight));
		}
		lapCoff.push_back(Triplet<float>(index, index, 1.0));
	}
}

void convertToEigenMatrix(vector<Vertex> a, MatrixXf& V) {
	V.resize(a.size(), 3);
	for (int i = 0; i < a.size(); i++)
		V.row(i) = a[i].Position;
	
}


void convertFromEigenMatrix(vector<Vertex> &a, MatrixXf V) {

	for (int i = 0; i < V.rows(); i++)
		a[i].Position = V.row(i);

}



//void calcAdjancyMatrix(Mesh* mesh, SparseMatrix<float>& adj) {
//	;
//}
