#include "pch.h"


using namespace Eigen;


static vector<Triplet<float>> originLaplacian,energe;
static SparseMatrix<float> LCoffMatrix, lengthKeep, lapNormalizeTrans, energyMatrix , energyMatrixTrans;
static VectorXf roiVertexMatrix, roiMatrix, LapCoordinate, solution;
static SimplicialLLT<SparseMatrix<float>> solver, lengthKeepSolver;


void dataInitial(Mesh* mesh) {
	
	int roiVerticeSize = mesh->roiVertices.size();
	convertToEigenMatrix(mesh->roiVertices, roiVertexMatrix);
	//calcAdjancyMatrix(mesh, adjancyMatrix);
 	calcLaplacianCoff(mesh, originLaplacian);

	//if use equation(5) in paper
	LCoffMatrix.resize(roiVerticeSize *3, roiVerticeSize *3);
	LCoffMatrix.reserve(10 * roiVerticeSize * 3);
	LCoffMatrix.setFromTriplets(originLaplacian.begin(), originLaplacian.end());

	LapCoordinate = LCoffMatrix * roiVertexMatrix;

	//calculate laplacian's normalized length
	for (int i = 0; i < roiVerticeSize; i++) {
		float currentLength;
		auto lap_1 = LapCoordinate[3 * i + 0];
		auto lap_2 = LapCoordinate[3 * i + 1];
		auto lap_3 = LapCoordinate[3 * i + 2];
		currentLength = sqrt(lap_1* lap_1 + lap_2* lap_2 + lap_3 * lap_3);
		mesh->lapLength.push_back(currentLength);
		//lapLength related to ROI indces
	}
	energe = originLaplacian;

	//handle constrained
	int contrainedSize = mesh->edgeConnectBody + mesh->edgeConnectHandle;
	for (int i = 0; i < contrainedSize; i++) {
	//for (int i = 0; i < 1; i++) {
		
		int roiLength = 3 * roiVerticeSize;
		energe.push_back(Triplet<float>(roiLength + i * 3 + 0, i * 3 + 0, 1.0));
		energe.push_back(Triplet<float>(roiLength + i * 3 + 1, i * 3 + 1, 1.0));
		energe.push_back(Triplet<float>(roiLength + i * 3 + 2, i * 3 + 2, 1.0));
	}

	energyMatrix.resize((roiVerticeSize+ contrainedSize) * 3, roiVerticeSize * 3);
	energyMatrix.reserve(10 * (roiVerticeSize + contrainedSize) * 3);


	energyMatrix.setFromTriplets(energe.begin(), energe.end());
	energyMatrixTrans = energyMatrix.transpose();

	
	//debugSolver.compute(LCoffMatrix.transpose() * LCoffMatrix);

	auto start = chrono::high_resolution_clock::now();

	solver.compute(energyMatrixTrans * energyMatrix);

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	cout << "Factorizating engery matrix use : " << duration.count() << " ms" << endl;
	cout << "ROI region have " << mesh->roiVertices.size() << " vertices\n";
}



void deform(Mesh* mesh,float x,float y,float z) {
	auto start = chrono::high_resolution_clock::now();
	int constrainedSize = mesh->edgeConnectBody + mesh->edgeConnectHandle;
	VectorXf b(3*(mesh->roiVertices.size()+ constrainedSize));
	//VectorXf b(3 * (mesh->roiVertices.size()+1));

	for (int i = 0; i < LapCoordinate.size(); i++) {
		b[i] = LapCoordinate[i];
	}

	//start movement
	for (int i = 0; i < mesh->handleIndice.size(); i++) {
		int index = mesh->handleIndice[i];
		mesh->vertices[index].Position[0] += x;
		mesh->vertices[index].Position[1] += y;
		mesh->vertices[index].Position[2] += z;
	}
	
	for (int i = 0; i < mesh->edgeConnectHandle; i++) {
		int index = mesh->handleIndice[i];
		mesh->roiVertices[i].Position[0] += x;
		mesh->roiVertices[i].Position[1] += y;
		mesh->roiVertices[i].Position[2] += z;
	}
	int constrainedBegin = LapCoordinate.size();
	for (int i = 0; i < constrainedSize; i++) {
		b[constrainedBegin + i * 3 + 0] = mesh->roiVertices[i].Position[0];
		b[constrainedBegin + i * 3 + 1] = mesh->roiVertices[i].Position[1];
		b[constrainedBegin + i * 3 + 2] = mesh->roiVertices[i].Position[2];
	}
	

	solution = solver.solve(energyMatrixTrans * b);
	convertFromEigenMatrix(mesh->roiVertices, solution);
	mesh->updateVertex();

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	std::cout << "Deform use : " << duration.count()   << " ms" << endl;
}




void calcLaplacianCoff(Mesh* mesh, vector<Triplet<float>>& lapCoff) {

	// this function use uniform laplacian
	for (int i = 0; i < mesh->roiVertices.size(); i++) {
		
		unsigned int degree = mesh->roiAdjMatrix[i].size();
		float weight = -1.0 / degree;

		for (int j = 0; j < degree; j++) {
			int j_index = mesh->roiAdjMatrix[i][j];
			
			for (int k = 0; k < 3; k++) {
				lapCoff.push_back(Triplet<float>(i * 3 + k, j_index*3 + k, weight));
			}
		}
		lapCoff.push_back(Triplet<float>(i * 3 + 0, i * 3 + 0, 1.0));
		lapCoff.push_back(Triplet<float>(i * 3 + 1, i * 3 + 1, 1.0));
		lapCoff.push_back(Triplet<float>(i * 3 + 2, i * 3 + 2, 1.0));
	}
}


void convertToEigenMatrix(vector<Vertex> a, VectorXf& V) {
	V.resize(a.size()* 3);

	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < 3; j++) {
			V[i*3+j] = a[i].Position[j];
		}
		
	}
	
}



void convertFromEigenMatrix(vector<Vertex> &a, VectorXf V) {

	for (int i = 0; i < V.rows()/3; i++) {
		
		a[i].Position[0] = V[i * 3 + 0];
		a[i].Position[1] = V[i * 3 + 1];
		a[i].Position[2] = V[i * 3 + 2];
	}
		

}


void converToROIMatrix(Mesh* mesh, VectorXf& V) {
	V.resize(mesh->vertices.size() * 3);
	for (int i = 0; i < mesh->ROIindice.size(); i++) {
		int index = mesh->ROIindice[i];
		V[index * 3 + 0] = mesh->vertices[index].Position[0];
		V[index * 3 + 1] = mesh->vertices[index].Position[1];
		V[index * 3 + 2] = mesh->vertices[index].Position[2];
	}
}


/*
s.b:
ROI contain handle Position and unconstrained part
handle Position

其他部分顶点直接不变，只计算ROI 中的 handle 和 unconstrained 部分
*/

