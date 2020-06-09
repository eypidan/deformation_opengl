#include "pch.h"

//camera
Camera camera;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//model
Model* magicCube;

//line shader
Shader* lineShader;

int main() {
    //fps calculate
    float globa_fps=0.0f;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "vgl-deformation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //configure global opengl state
    glEnable(GL_DEPTH_TEST);
    
    //shader init
    Shader modelShader("./shaderFiles/modelVertex.glsl", "./shaderFiles/modelFragment.glsl");

    // load model
    //Model magicCube_0("./models/cublic.obj");
    Model magicCube_0("./models/bunny.off");
    //Model magicCube_0("./models/magicCube/magicCube.obj");
    magicCube = &magicCube_0;
    //define light source
    lightSource light;
    light.lightColor[0] = 1; light.lightColor[1] = 1; light.lightColor[2] = 1;
    light.lightPos[0] = 10; light.lightPos[1] = 100; light.lightPos[2] = 10;
    //camera
    

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //calculate fps-for debug
        //globa_fps = 1.0 / deltaTime;
        //printf("fps:%f\n", globa_fps);


        processInput(window);

        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderModel(modelShader, magicCube, light, camera);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}