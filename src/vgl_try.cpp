#include "pch.h"

//camera
Camera camera;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


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
    Model magicCube("./models/magicCube/magicCube.obj");

    //define light source
    lightSource light;
    light.lightColor[0] = 1; light.lightColor[1] = 1; light.lightColor[2] = 1;
    light.lightPos[0] = 1; light.lightPos[1] = 200; light.lightPos[2] = 200;
    //camera
    
    //debug
    float verticess[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.9f, 0.0f  // top   
    };
    struct v {
        Eigen::Vector3f a;
    };
    vector<v> vertices;
    for (int i = 0; i < 3; i++) {
        struct v ts;

        ts.a[0] = verticess[i*3];
        ts.a[1] = verticess[i*3+1];
        ts.a[2] = verticess[i*3+2];
        vertices.push_back(ts);
    }
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v)*vertices.size(), (void*)&vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(v), (void*)(vertices[0].a.data() - (float*)&vertices[0]));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

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

        ////correct order scale, rotate,translate T*R*S*MODEL
        //Eigen::Affine3f r_model = create_rotation_matrix(x_angles, y_angles, z_angles);
        //Eigen::Affine3f t_model(Eigen::Translation3f(Eigen::Vector3f(0, 0, -20)));
        //Eigen::Affine3f s_model(Eigen::Scaling(Eigen::Vector3f(0.8, 0.8, 0.8)));
        //Eigen::Matrix4f transform_model = (t_model * r_model * s_model).matrix();
        //glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 projection = camera.GetPerspectiveMatrix();

        //modelShader.use();
        //modelShader.setMat4("model", transform_model);
        //modelShader.setMat4("view", view);
        //modelShader.setMat4("projection", projection);
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        renderModel(modelShader, &magicCube, light, camera);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}