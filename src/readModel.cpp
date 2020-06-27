#include "pch.h"

using namespace Eigen;


static Eigen::Vector3f yellow , black, red,green;
//static vector<unsigned int> bunnyControlPoints = { 195,127,71,339,346,581,577,410,587,629,514,260,330,481,717,437,570,165 };
vector<unsigned int> bunnyControlPoints = { 430 };

void Mesh::Draw(Shader shader) {
	
	// draw line
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);
	shader.setVec3("color", black);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	//draw mesh
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader.setVec3("color", yellow);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	shader.use();
	//draw point
	glBindVertexArray(VAO_point[0]);
	shader.setVec3("color", red);
	glDrawArrays(GL_POINTS, 0, this->lapVetices.size());

	glBindVertexArray(VAO_point[1]);
	shader.setVec3("color", green);
	glDrawArrays(GL_POINTS, 0, this->edgeVertices.size());
	//set to defaulte
	glBindVertexArray(0);

}


Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<vector<int>> adjMatrix, vector<unsigned int> ROIv) {
	this->vertices = vertices;
	this->indices = indices;
	this->adjMatrix = adjMatrix;
	this->ROIindice = ROIv;

	//handle point
	for (int i = 0; i < this->ROIindice.size(); i++) {
		int index = this->ROIindice[i];
		float y = this->vertices[index].Position[1];
		float x = this->vertices[index].Position[0];
		float z = this->vertices[index].Position[2];
		if(bunny)
			if (y > 0.1623) this->handleIndice.push_back(index);
		if(armadillo)
			if (x < -0.32 && y > 0.22 && z > 0.18) this->handleIndice.push_back(index);
	// (x < -0.33) && (y > 0.242) && (z > 0.09)
	}
	
	for (int i = 0; i < this->handleIndice.size(); i++) {
		ROIindice.erase(std::remove(ROIindice.begin(), ROIindice.end(), this->handleIndice[i]), ROIindice.end());
	}

	// *** find edges vertex in roiVertices *** BEGIN
	
	for (int i = 0; i < ROIindice.size(); i++) {
		int index = ROIindice[i];
	}

	//build map relation between ROI and model's vertices 
	for (int i = 0; i < this->ROIindice.size(); i++) {
		int index = this->ROIindice[i];
		this->roiMap[index] = i;
	}

	//use adjency matrix to find the edge 
	for (int i = 0; i < this->ROIindice.size(); i++) {
		int index = this->ROIindice[i];
		bool flag = true;
		for (int j = 0; j < this->adjMatrix[index].size() && flag; j++) {
			int adjVertexKey = this->adjMatrix[index][j];
			if (this->roiMap.find(adjVertexKey) == this->roiMap.end()) {
				edgeIndice.push_back(index); //find edge veretx
				flag = false;
			}
		}
	}
	//divide edge connect to handle and edge connect to body
	vector<int> edgeHandle, edgeBody; // the edge besides handle part and body part
	for (int i = 0; i < edgeIndice.size(); i++) {
		int index = edgeIndice[i];
		if (vertices[index].Position[0] > -0.3435 && vertices[index].Position[1] < 0.21877)
			edgeBody.push_back(index);
		else
			edgeHandle.push_back(index);
	}
	edgeIndice.clear();
	edgeIndice.assign(edgeHandle.begin(), edgeHandle.end());
	edgeIndice.insert(edgeIndice.end(), edgeBody.begin(), edgeBody.end());
	
	// *** find edges vertex in roiVertices *** END

	//the vertices that are unconstrianed
	lapIndices = ROIindice;
	for (int i = 0; i < this->edgeIndice.size(); i++) {
		lapIndices.erase(std::remove(lapIndices.begin(), lapIndices.end(), this->edgeIndice[i]), lapIndices.end());
	}

	//ADD BACK TO ROI
	ROIindice.clear();
	ROIindice.assign(edgeIndice.begin(), edgeIndice.end());
	ROIindice.insert(ROIindice.end(), lapIndices.begin(), lapIndices.end());
	
	//build roi Vertices' adjency matrix
	roiMap.clear();
	for (int i = 0; i < this->ROIindice.size(); i++) {
		int index = this->ROIindice[i];
		this->roiMap[index] = i;
	}

	//use adjency matrix to find the edge 

	for (int i = 0; i < this->ROIindice.size(); i++) {
		int index = this->ROIindice[i];
		vector<unsigned int> currentAdj;
		for (int j = 0; j < this->adjMatrix[index].size(); j++) {
			int adjVertexKey = this->adjMatrix[index][j];
			if (this->roiMap.find(adjVertexKey) != this->roiMap.end()) {
				currentAdj.push_back(this->roiMap[adjVertexKey]);
			}
		}
		roiAdjMatrix.push_back(currentAdj);
	}
	//divi
	//assign vertex value to mesh object
	for (int i = 0; i < edgeIndice.size(); i++) {
		int index = edgeIndice[i];
		this->edgeVertices.push_back(this->vertices[index]);
	}
	for (int i = 0; i < ROIindice.size(); i++) {
		int index = ROIindice[i];
		this->roiVertices.push_back(this->vertices[index]);
	}
	for (int i = 0; i < lapIndices.size(); i++) {
		int index = lapIndices[i];
		this->lapVetices.push_back(this->vertices[index]);
	}
	this->edgeConnectBody = edgeBody.size();
	this->edgeConnectHandle = edgeHandle.size();


	if (bunny || armadillo) {
		
		glGenVertexArrays(3, VAO_point);
		glGenBuffers(3, VBO_point);

		for (int i = 0; i < 3; i++) {
			glBindVertexArray(VAO_point[i]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO_point[i]);
			
			if (i == 0) {
				glBufferData(GL_ARRAY_BUFFER, lapVetices.size() * sizeof(Vertex), &lapVetices[0], GL_DYNAMIC_DRAW);
				// position 
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(lapVetices[0].Position.data() - (float*)&lapVetices[0]));
				// normal
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(lapVetices[0].Normal.data() - (float*)&lapVetices[0]));
			}
			
			if (i == 1) {
				glBufferData(GL_ARRAY_BUFFER, edgeVertices.size() * sizeof(Vertex), &edgeVertices[0], GL_DYNAMIC_DRAW);
				// position 
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(edgeVertices[0].Position.data() - (float*)&edgeVertices[0]));
				// normal
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(edgeVertices[0].Normal.data() - (float*)&edgeVertices[0]));
			}
		}
		

		glBindVertexArray(0);
	}
	//create buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);


	// position 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(vertices[0].Position.data()- (float*)&vertices[0])); 
	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(vertices[0].Normal.data()- (float*)& vertices[0]));

	//// element color
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Eigen::Vector3f), (void*)(fragmentColor[0].data() - (float*)&fragmentColor[0]));
	
	glBindVertexArray(0);
	yellow << 0.4 ,0.4 ,0.6;
	green << 0.0, 1.0, 0.0;
	black << 0.0, 0.0, 0.0;
	red << 1.0, 0.0, 0.0;
	//data prepare for deformation
	dataInitial(this);
}

void Mesh::updateVertex() {

	for (int i = 0; i < this->roiVertices.size(); i++) {
		int formalIndex = this->ROIindice[i];
		this->vertices[formalIndex] = this->roiVertices[i];
	}

	for (int i = 0; i < this->lapIndices.size(); i++) {
		int start = edgeConnectHandle + edgeConnectBody;
		this->lapVetices[i] = this->roiVertices[start+i];
	}

	for (int i = 0; i < edgeConnectHandle; i++) {

		this->edgeVertices[i] = this->roiVertices[i];
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

	// position 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(vertices[0].Position.data() - (float*)&vertices[0]));
	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(vertices[0].Normal.data() - (float*)&vertices[0]));
	//// vertex color
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(vertices[0].Color.data() - (float*)&vertices[0]));
	
	for (int i = 0; i < 3; i++) {
		glBindVertexArray(VAO_point[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_point[i]);

		if (i == 0) {
			glBufferData(GL_ARRAY_BUFFER, lapVetices.size() * sizeof(Vertex), &lapVetices[0], GL_DYNAMIC_DRAW);
			// position 
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(lapVetices[0].Position.data() - (float*)&lapVetices[0]));
		}

		if (i == 1) {
			glBufferData(GL_ARRAY_BUFFER, edgeVertices.size() * sizeof(Vertex), &edgeVertices[0], GL_DYNAMIC_DRAW);
			// position 
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(edgeVertices[0].Position.data() - (float*)&edgeVertices[0]));
		}
	}

	glBindVertexArray(0);


}


void Model::Draw(Shader shader) {
	for (int i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader);
}

Model::Model(string const& path) {
	//auto start = chrono::high_resolution_clock::now();
	//read files
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR::ASSIMP:: " << import.GetErrorString() << endl;
		return ;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	//process node recursively
	processNode(scene->mRootNode, scene);
	//auto stop = chrono::high_resolution_clock::now();
	//auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	//cout << "Load model use : " << duration.count()/1000000.0 << " s" << endl;
}


void Model::processNode(aiNode* node, const aiScene* scene) {
	// mesh in node
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, scene));
	}
	//recursively possess other children nodes
	for (int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<vector<int>> adjMatrix;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex v;
		v.Position[0] = mesh->mVertices[i].x;
		v.Position[2] = mesh->mVertices[i].z;
		v.Position[1] = mesh->mVertices[i].y;

		if (mesh->mNormals != NULL) {
			v.Normal[0] = mesh->mNormals[i].x;
			v.Normal[1] = mesh->mNormals[i].y;
			v.Normal[2] = mesh->mNormals[i].z;
		}
		else {
			//Vector3f a;
		}

		vertices.push_back(v);
	}

	//face
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		
		for (int j = 0; j < face.mNumIndices; j++) {
			
			indices.push_back(face.mIndices[j]);
		}
			
	}

	////adj matrix
	//// O(V*F) too slow 
	//for (int i = 0; i < mesh->mNumVertices; i++){
	//	vector<int> a;
	//	
	//	for (int j = 0; j < mesh->mNumFaces; j++) {
	//		aiFace face = mesh->mFaces[j];

	//		if (face.mIndices[0] == i || face.mIndices[1] == i || face.mIndices[2] == i) {
	//			a.push_back(face.mIndices[0]);
	//			a.push_back(face.mIndices[1]);
	//			a.push_back(face.mIndices[2]);
	//		}
	//	}

	//	adjMatrix.push_back(a);
	//}

	//adj matrix
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vector<int> a;
		adjMatrix.push_back(a);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < 3; j++) {
			int index = face.mIndices[j];
			int index_1 = face.mIndices[(j + 1) % 3];
			int index_2 = face.mIndices[(j + 2) % 3];
			adjMatrix[index].push_back(index_1);
			adjMatrix[index].push_back(index_2);
		}
	}

	for (int i = 0; i < mesh->mNumVertices; i++) { //remove rundant element
		std::sort(adjMatrix[i].begin(), adjMatrix[i].end());
		auto uniIt = std::unique(adjMatrix[i].begin(), adjMatrix[i].end());
		adjMatrix[i].erase(uniIt, adjMatrix[i].end());
	}

	//ROI region
	vector<unsigned int> ROIindice;
	if (bunny) {
		//bunny's condition :x0 < -0.055 && y0 > 0.146 && z0 < -0.003
		for (int i = 0; i < mesh->mNumVertices; i++) {
			if (vertices[i].Position[0] < -0.055 && vertices[i].Position[1]>0.146 && vertices[i].Position[2] < -0.003)
				ROIindice.push_back(i);
		}
	}
	if (armadillo) {
		for (int i = 0; i < mesh->mNumVertices; i++) {
			if (vertices[i].Position[0] < -0.25 && vertices[i].Position[1] > 0.048)
				ROIindice.push_back(i);
		}
	}
	


	return Mesh(vertices, indices, adjMatrix, ROIindice);
}

