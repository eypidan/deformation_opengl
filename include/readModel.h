#ifndef READMODEL_H
#define READMODEL_H

using namespace std;

struct Vertex {
	Eigen::Vector3f Position;
	Eigen::Vector3f Normal;
};


class Mesh {
private:
	vector<unsigned int> indicesControl;
	vector<Vertex> verticesControl;

	vector<unsigned int>indices;
	unsigned int VAO;
	unsigned int VBO, EBO;
	unsigned int VAO_point[3], VBO_point[3];
public:
	vector<float> lapLength;
	vector<Vertex> vertices;
	vector<vector<int>> adjMatrix;
	vector<unsigned int> handleIndice;
	
	vector<unsigned int> ROIindice; //the index of vertex in roi region 
	vector<Vertex> roiVertices;
	vector<vector<unsigned int>> roiAdjMatrix;

	vector<unsigned int> edgeIndice; //edge vertices
	vector<Vertex> edgeVertices;

	vector<unsigned int> lapIndices; //unconstrained points in energy matrix
	vector<Vertex> lapVetices;

	unsigned int edgeConnectHandle;
	unsigned int edgeConnectBody;
	map<int, int> roiMap;
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<vector<int>> adjMatrix, vector<unsigned int> ROIv);
	void updateVertex();
	void Draw(Shader shader);
};

class Model {
public:
	Model(string const& path);
	void Draw(Shader shader);
	vector<Mesh> meshes;
private:
	string directory;

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	
};

Eigen::Affine3f create_rotation_matrix(double ax, double ay, double az);

#endif // READMODEL_H
