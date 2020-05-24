#ifndef MISCFUNCTION_H
#define MISCFUNCTION_H


void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int TextureFromFile(const char* path, const std::string & directory);

struct lightSource {
	glm::vec3 lightPos;
	glm::vec3 lightColor;
};

#endif