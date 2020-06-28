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

//window
GLFWwindow* window;

//state flag
bool bunny = false;
bool armadillo = false;
int main() {
    //fps calculate
    float globa_fps=0.0f;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "vgl-deformation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);


    //shader init
    Shader modelShader("./shaderFiles/modelVertex.glsl", "./shaderFiles/modelFragment.glsl");

    
    //define light source
    lightSource light;
    light.lightColor[0] = 1; light.lightColor[1] = 1; light.lightColor[2] = 1;
    light.lightPos[0] = 10; light.lightPos[1] = 100; light.lightPos[2] = 10;
    
    //first hide window
    glfwHideWindow(window);
    int select;
    cout << "Laplacian Mesh Editing\n\nUse A,D to rotate the model\nUse scroll to adjust the distance of the model from the screen\n";
    cout << "Use up,down,left,right arrows in keyboard to move the handle part in model\n";
    cout << "Use ESC to exit the program\n\n";
    cout << "Select target model" << endl <<"1. bunny example.\n2. armadillo example\nPlease input:";
    cin >> select;
    //select = 1;
    string path = "./models/armadillo_easy.off";
    switch(select) {
    case 1:
        bunny = true;
        // load model
        //Model magicCube_0("./models/cublic.obj");
        path = "./models/bunny.off";
        break;
    case 2:
        armadillo = true;
        path = "./models/armadillo_easy.off";
        break;
    default:
        armadillo = true;
        cout << "Use armadillo as default\n";
        break;
    };

    Model magicCube_0(path);
    //Model magicCube_0("./models/magicCube/magicCube.obj");
    magicCube = &magicCube_0;
    
    //show window after input
    glfwShowWindow(window);
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