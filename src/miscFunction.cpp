#include "pch.h"

//model rotate
float x_angles = 0.0f;
float y_angles = 0.0f;
float z_angles = 0.0f;
float height=-5;
controlMove controlMove_0;

static bool oldSpace = GLFW_RELEASE;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		x_angles += 0.05f;
		//camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		x_angles -= 0.05f;
		//camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		y_angles += 0.05f;
		//camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		y_angles -= 0.05f;
		//camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		z_angles -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		z_angles += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		controlMove_0.x += 5;
		//test(&magicCube->meshes[0]);
	}
		
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		controlMove_0.x -= 5;
		//test(&magicCube->meshes[0]);
	}
		
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		deform(&magicCube->meshes[0], -0.01, 0.02, -0.01);
		//test(&magicCube->meshes[0]);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		deform(&magicCube->meshes[0], -0.01, 0.01, 0.01);
	}
	

	bool spaceAction = glfwGetKey(window, GLFW_KEY_SPACE);
	if (spaceAction == GLFW_RELEASE && oldSpace == GLFW_PRESS) {

		deform(&magicCube->meshes[0], - 0.01, 0.01, 0.01);
	}
	oldSpace = spaceAction;
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	mouseX = xpos;
	mouseY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
		height += 0.2;
	}
	else {
		height -= 0.2;
	}
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);
}

unsigned int TextureFromFile(const char* path, const std::string & directory) {
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}