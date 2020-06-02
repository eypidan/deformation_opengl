#ifndef READMODEL_H
#define READMODEL_H

using namespace std;

struct Vertex {
	Eigen::Vector3f Position;
	Eigen::Vector3f Normal;
	Eigen::Vector2f TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
private:
	vector<Vertex> vertices;
	vector<unsigned int>indices;
	vector<Texture> textures;
	unsigned int VAO;
	unsigned int VBO, EBO;
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader shader);
};

class Model {
public:
	Model(string const& path);
	void Draw(Shader shader);

private:
	//model data
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

Eigen::Affine3f create_rotation_matrix(double ax, double ay, double az);

#endif // READMODEL_H
