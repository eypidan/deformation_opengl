#include "pch.h"

using namespace Eigen;


static vector<Triplet<float>> originLaplacian,energe;
static SparseMatrix<float> LCoffMatrix, lengthKeep, lapNormalizeTrans, energyMatrix , energyMatrixTrans;
static VectorXf roiVertexMatrix, roiMatrix, LapCoordinate, solution;
static SimplicialLLT<SparseMatrix<float>> solver, lengthKeepSolver;


unsigned int selectedPoint;


void dataInitial(Mesh* mesh) {
	//add roi data into 
	for (int i = 0; i < mesh->ROIindice.size(); i++) {

		int index = mesh->ROIindice[i];
		Vertex currentV = mesh->vertices[index];
		mesh->roiVertices.push_back(currentV);
		mesh->roiMap[index] = i;
	}

	//roi vertice's adjancy Matrix
	for (int i = 0; i < mesh->ROIindice.size(); i++) {
		int index = mesh->ROIindice[i];
		
		vector<int> currentAdj;
		for (int j = 0; j < mesh->adjMatrix[index].size(); j++) {
			int formalKey = mesh->adjMatrix[index][j];
			if(mesh->roiMap.find(formalKey) != mesh->roiMap.end()){ //find
				currentAdj.push_back(mesh->roiMap[formalKey]);
			}
		}
		mesh->roiAdjMatrix.push_back(currentAdj);
	}

	int roiVerticeSize = mesh->roiVertices.size();

	convertToEigenMatrix(mesh->roiVertices, roiVertexMatrix);
	//calcAdjancyMatrix(mesh, adjancyMatrix);
 	calcLaplacianCoff(mesh, originLaplacian);

	////test|
	//LCoffMatrix.resize(roiVerticeSize * 3, roiVerticeSize * 3);
	//LCoffMatrix.reserve(10 * roiVerticeSize * 3);
	//LCoffMatrix.setFromTriplets(originLaplacian.begin(), originLaplacian.end());
	//VectorXf test = LCoffMatrix * roiVertexMatrix;
	//convertFromEigenMatrix(mesh->roiVertices, test);
	//mesh->updateVertex();
	//return;
	////test|

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
	
	////calculate energy matrix
	//energe = calcEnergyCoff(mesh, originLaplacian);

	////laplacian's length keep
	//for (int i = 0; i < mesh->formalIndice.size(); i++) {
	//	int index = mesh->formalIndice[i];
	//	originLaplacian.push_back(Triplet<float>(index * 3 + 0, index * 3 + 0, 1.0));
	//	originLaplacian.push_back(Triplet<float>(index * 3 + 1, index * 3 + 1, 1.0));
	//	originLaplacian.push_back(Triplet<float>(index * 3 + 2, index * 3 + 2, 1.0));
	//}
	//lengthKeep.resize(roiVerticeSize * 3, roiVerticeSize * 3);
	//lengthKeep.setFromTriplets(originLaplacian.begin(), originLaplacian.end());
	//lapNormalizeTrans = lengthKeep.transpose();
	//lengthKeepSolver.compute(lapNormalizeTrans * lengthKeep);

	//handle constrained
	for (int i = 0; i < mesh->handleIndice.size(); i++) {
		int index = mesh->handleIndice[i];
		int index_in_roi = mesh->roiMap[index];
		int roiLength = 3 * roiVerticeSize;
		energe.push_back(Triplet<float>(roiLength + i * 3 + 0, i * 3 + 0, 1.0));
		energe.push_back(Triplet<float>(roiLength + i * 3 + 1, i * 3 + 1, 1.0));
		energe.push_back(Triplet<float>(roiLength + i * 3 + 2, i * 3 + 2, 1.0));
	}


	int handleSize = mesh->handleIndice.size();
	//L = LCoffMatrix* roiVertexMatrix;
	energyMatrix.resize((roiVerticeSize+ handleSize) * 3, roiVerticeSize * 3);
	energyMatrix.reserve(10 * (roiVerticeSize + handleSize) * 3);
	energyMatrix.setFromTriplets(energe.begin(), energe.end());
	energyMatrixTrans = energyMatrix.transpose();
	solver.compute(energyMatrixTrans * energyMatrix);

	//auto start = chrono::high_resolution_clock::now();


	//auto stop = chrono::high_resolution_clock::now();
	//auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	//std::cout << "Factorizating engery matrix use : " << duration.count() / (double)1000000.0 << " s" << endl;
}


vector<Triplet<float>> calcEnergyCoff(Mesh *mesh, vector<Triplet<float>>& lapCoff) {

	std::vector<MatrixXf> Ts;
	Ts.resize(mesh->ROIindice.size());

	//add self to adjMatrix
	for (int i = 0; i < mesh->adjMatrix.size(); i++) {
		mesh->adjMatrix[i].push_back(i);
		std::sort(mesh->adjMatrix[i].begin(), mesh->adjMatrix[i].end());
	}

	for (int i = 0; i < mesh->ROIindice.size(); i++) {
		int index = mesh->ROIindice[i];
		auto adj = mesh->adjMatrix[index];
		MatrixXf At(7, adj.size() * 3);

		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < adj.size() * 3; col++) {
				At(row, col) = 0.0f;
			}
		}

		for (int j = 0; j < adj.size(); ++j) {
			int k = adj[j];

			float vk[3];
			
			vk[0] = mesh->vertices[index].Position[0];
			vk[1] = mesh->vertices[index].Position[1];
			vk[2] = mesh->vertices[index].Position[2];

			const int x = 0;
			const int y = 1;
			const int z = 2;

			At(0, j * 3 + 0) = +vk[x];
			At(1, j * 3 + 0) = 0;
			At(2, j * 3 + 0) = +vk[z];
			At(3, j * 3 + 0) = -vk[y];
			At(4, j * 3 + 0) = +1;
			At(5, j * 3 + 0) = 0;
			At(6, j * 3 + 0) = 0;

			At(0, j * 3 + 1) = +vk[y];
			At(1, j * 3 + 1) = -vk[z];
			At(2, j * 3 + 1) = 0;
			At(3, j * 3 + 1) = +vk[x];
			At(4, j * 3 + 1) = 0;
			At(5, j * 3 + 1) = +1;
			At(6, j * 3 + 1) = 0;

			At(0, j * 3 + 2) = +vk[z];
			At(1, j * 3 + 2) = +vk[y];
			At(2, j * 3 + 2) = -vk[x];
			At(3, j * 3 + 2) = 0;
			At(4, j * 3 + 2) = 0;
			At(5, j * 3 + 2) = 0;
			At(6, j * 3 + 2) = 1;
		}

		MatrixXf invprod = (At * At.transpose()).inverse();
		MatrixXf pseudoinv = invprod * At;
		Ts[i] = pseudoinv;
	}

	//derive from uniform matrix
	std::vector<Triplet<float>> result = lapCoff;

	std::map<int, double> row;
	
	for (int i = 0; i < (mesh->ROIindice.size() * 3); i++) {
		row.clear();
		int index = mesh->ROIindice[i / 3];
		
		// get delta coordinates for the vertex.
		double dx = LapCoordinate[index * 3 + 0];
		double dy = LapCoordinate[index * 3 + 1];
		double dz = LapCoordinate[index * 3 + 2];

		

		MatrixXf T = Ts[i / 3];

		VectorXf s = T.row(0);
		VectorXf h1 = T.row(1);
		VectorXf h2 = T.row(2);
		VectorXf h3 = T.row(3);
		VectorXf tx = T.row(4);
		VectorXf ty = T.row(5);
		VectorXf tz = T.row(6);

		if ((i % 3) == 0) { // x case.
			for (int j = 0; j < T.row(0).size(); ++j) {
				int p = j % 3; // p=0,1,2
				int q = j / 3;  //q, the indice of it and its neighbors
				int r = mesh->adjMatrix[index][q];

				row[p + 3 * r] -= dx * (+s[j]);
				row[p + 3 * r] -= dy * (-h3[j]);
				row[p + 3 * r] -= dz * (+h2[j]);
			}
		}
		else if ((i % 3) == 1) { // y case.
			for (int j = 0; j < T.row(0).size(); ++j) {
				int p = j % 3;
				int q = j / 3;
				int r = mesh->adjMatrix[index][q];

				row[p + 3 * r] -= dx * (+h3[j]);
				row[p + 3 * r] -= dy * (+s[j]);
				row[p + 3 * r] -= dz * (-h1[j]);
			}
		}
		else if ((i % 3) == 2) { // z case.
			for (int j = 0; j < T.row(0).size(); ++j) {
				int p = j % 3;
				int q = j / 3;
				int r = mesh->adjMatrix[index][q];

				row[p + 3 * r] -= dx * (-h2[j]);
				row[p + 3 * r] -= dy * (+h1[j]);
				row[p + 3 * r] -= dz * (+s[j]);

			}
		}

		for (const auto& p : row) {
			result.push_back(Triplet<float>(index + i % 3, p.first, p.second));
		}
	}
	return result;
}


void deform(Mesh* mesh) {
	auto start = chrono::high_resolution_clock::now();

	selectedPoint = bunnyControlPoints[0];
	for (int i = 0; i < mesh->handleIndice.size(); i++) {
		int index = mesh->handleIndice[i];
		int index_in_roi = mesh->roiMap[index];
		mesh->roiVertices[index_in_roi].Position[0] += -0.01;
		mesh->roiVertices[index_in_roi].Position[1] += 0.01;
		mesh->roiVertices[index_in_roi].Position[2] += 0.01;
	}
	
	/*for (int i = 0; i < mesh->ROIindice.size(); i++) {
		int index = mesh->ROIindice[i];
		mesh->vertices[index].Position[0] += 0;
		mesh->vertices[index].Position[1] += 0;
		mesh->vertices[index].Position[2] += 0;
	}*/

	convertToEigenMatrix(mesh->vertices, roiVertexMatrix);

	solution = solver.solve(energyMatrixTrans * energyMatrix * roiVertexMatrix);
	convertFromEigenMatrix(mesh->vertices, solution);
	
	std::cout << "DEBUG:: change: " << (solution - roiVertexMatrix).norm() << endl;

	mesh->updateVertex();

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	std::cout << "Deform use : " << duration.count() / 1000000.0 << " s" << endl;
}

void calcLaplacianCoff(Mesh* mesh, vector<Triplet<float>>& lapCoff) {

	// this function use uniform laplacian
	for (int i = 0; i < mesh->roiVertices.size(); i++) {
		
		unsigned int degree = mesh->roiAdjMatrix[i].size();
		float weight = -1.0 / degree;

		for (int j = 0; j < degree; j++) {
			int j_index = mesh->roiAdjMatrix[i][j];
			
			for (int k = 0; k < 3; k++) {
				lapCoff.push_back(Triplet<float>(i * 3 + k, j_index * 3 + k, -0.01));
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

//void calcAdjancyMatrix(Mesh* mesh, SparseMatrix<float>& adj) {
//	;
//}

/*
s.b:
ROI contain handle Position and unconstrained part
handle Position

其他部分顶点直接不变，只计算ROI 中的 handle 和 unconstrained 部分
*/

