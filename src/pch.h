
// settings
#define SCR_WIDTH  1000
#define SCR_HEIGHT  800

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>


#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include "miscFunction.h"
#include "shaderLoad.h"
#include "camera.h"
#include "readModel.h"
#include "renderFunction.h"
#include "deformation.h"

//model
extern Model *magicCube;

//camera
extern Camera camera;

// timing
extern float deltaTime;
extern float lastFrame;

//model rotate
extern float x_angles;
extern float y_angles;
extern float z_angles;

//control point move
extern controlMove controlMove_0;

