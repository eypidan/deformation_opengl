#include "pch.h"

void renderModel(Shader shader, Model *object,  lightSource light, Camera camera) {

	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
	
	
	//correct order scale, rotate,translate T*R*S*MODEL
	//however,glm is reverse..... 
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
	
	model = glm::rotate(model, x_angles, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, y_angles, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, z_angles, glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	shader.setVec3("lightColor", light.lightColor);
	shader.setVec3("lightPos", light.lightPos);
	shader.setVec3("viewPos", camera.Position);

	object->Draw(shader);
}