#include "pch.h"

void renderModel(Shader shader, Model *object,  lightSource light, Camera camera) {

	shader.use();
	Eigen::Matrix4f model;
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetPerspectiveMatrix();
	
	
	//correct order scale, rotate,translate T*R*S*MODEL
	Eigen::Affine3f r_model = create_rotation_matrix(x_angles, y_angles,z_angles);
	Eigen::Affine3f t_model(Eigen::Translation3f(Eigen::Vector3f(0, 0, -20)));
	Eigen::Affine3f s_model(Eigen::Scaling(Eigen::Vector3f(0.05, 0.05, 0.05)));
	Eigen::Matrix4f transform_model = (t_model * r_model * s_model).matrix();

	model = transform_model;
	//cout << view << endl;
	//cout << projection << endl;
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	shader.setVec3("lightColor", light.lightColor);
	shader.setVec3("lightPos", light.lightPos);
	shader.setVec3("viewPos", camera.Position);

	object->Draw(shader);
}

Eigen::Affine3f create_rotation_matrix(double ax, double ay, double az) {
	Eigen::Affine3f rx =
		Eigen::Affine3f(Eigen::AngleAxisf(ax, Eigen::Vector3f(1, 0, 0)));
	Eigen::Affine3f ry =
		Eigen::Affine3f(Eigen::AngleAxisf(ay, Eigen::Vector3f(0, 1, 0)));
	Eigen::Affine3f rz =
		Eigen::Affine3f(Eigen::AngleAxisf(az, Eigen::Vector3f(0, 0, 1)));
	return rz * ry * rx;
}
