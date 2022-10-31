#include <iostream>
#include <string.h>

#include <GL/glew.h> //Cross-platform extension to allow different OS's
#include <GLFW/glfw3.h>//Actual OpenGL Library to GFX API

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

void CreateTriangle()
{
    GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //---
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);//Enable 0? from prev line
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //---

}

int main()
{
    //initialize GLFW
    if (!glfwInit()) {
        std::cout << "GLFW initialisation failed!";
        glfwTerminate();
        return 1;//1 is error/failed/crashed
    }

    //---Setup GLDW window properties---
    //OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Dissallows backward compatability
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //---End properties---

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        std::cout << "GLFW Window itself failed";
        glfwTerminate();
        return 1; //Fail
    }

    // Get Buffer Size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Set context for glew to use
    glfwMakeContextCurrent(mainWindow);

    //Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW Cross Compatible failed!";
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }


    //Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Loop until window closes
    while (!glfwWindowShouldClose(mainWindow)) {
        //Get + handle user input events
        glfwPollEvents();

        //clear window
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
