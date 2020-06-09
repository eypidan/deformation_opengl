#include "pch.h"

using namespace Eigen;


static Eigen::Vector3f yellow , black, red;
static vector<unsigned int> bunnyControlPoints = { 195,127,71,339,346,581,577,410,587,629,514,260,330,481,717,437,570,165 };


void Mesh::Draw(Shader shader) {
	
	// draw mesh
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);
	shader.setVec3("color", black);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	//draw line
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader.setVec3("color", yellow);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	shader.use();
	//draw point
	glBindVertexArray(VAO_point);
	shader.setVec3("color", red);
	glDrawArrays(GL_POINTS, 0, this->verticesControl.size());
	//set to default
	glBindVertexArray(0);

}


Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

	if (bunny) {
		this->indicesControl = bunnyControlPoints;
		for (int i = 0; i < this->indicesControl.size(); i++) 
			this->verticesControl.push_back(this->vertices[this->indicesControl[i]]);
		
		glGenVertexArrays(1, &VAO_point);
		glGenBuffers(1, &VBO_point);


		glBindVertexArray(VAO_point);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_point);
		glBufferData(GL_ARRAY_BUFFER, verticesControl.size() * sizeof(Vertex), &verticesControl[0], GL_STATIC_DRAW);
	
		// position 
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(verticesControl[0].Position.data() - (float*)&verticesControl[0]));
		// normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(verticesControl[0].Normal.data() - (float*)&verticesControl[0]));

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
	black << 0.0, 0.0, 0.0;
	red << 1.0, 0.0, 0.0;
}

void Mesh::updateVertex() {
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

	glBindVertexArray(0);
}


void Model::Draw(Shader shader) {
	for (int i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader);
}

Model::Model(string const& path) {
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

	return Mesh(vertices, indices);
}
