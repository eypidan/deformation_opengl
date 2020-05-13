#ifndef MISCFUNCTION_H
#define MISCFUNCTION_H


void processInput(GLFWwindow* window);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
unsigned int TextureFromFile(const char* path, const std::string & directory);

#endif