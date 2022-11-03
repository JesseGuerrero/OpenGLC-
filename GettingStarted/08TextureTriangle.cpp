#include <stdio.h>
#include "ogldev/jesse/jesseShader.h"
#include <GL/freeglut.h>
#include "ogldev/ogldev_math_3d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "learnopengl/stb_image.h"
#include "learnopengl/filesystem.h"


unsigned int VBO, VAO, EBO;
unsigned int texture;
Shader *ourShader;

static void CreateVertexBuffer()
{
    ourShader = (Shader*)(new Shader("Shaders/TextureLearnOpenGLSampleVertex.glsl", "Shaders/TextureLearnOpenGLSampleFragment.glsl"));
    float Vertices[] = {
            // positions                         // colors                    // texture coords
            -0.5f,  -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   -1.0f, -1.0f, // bottom left
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, -1.0f, // bottom right
            0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // top
    };

    unsigned int indices[] = {
            0, 1, 2, // first triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                // color attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // texture coord attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                // load and create a texture
                // -------------------------
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
                // set the texture wrapping parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // set texture filtering parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // load image, create texture and generate mipmaps
                int width, height, nrChannels;
                // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
                unsigned char *data = stbi_load(FileSystem::getPath("Images/container.jpg").c_str(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cout << "Failed to load texture" << std::endl;
                }
                stbi_image_free(data);

}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // render container
    (*ourShader).use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    int width = 800;
    int height = 600;
    glutInitWindowSize(width, height);

    int x = 100;
    int y = 100;
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("Tutorial 03");
    printf("window id: %d\n", win);
    printf("GL: %s\nGLUT: %d\n", glGetString(GL_VERSION), glutGet(GLUT_VERSION));


    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    CreateVertexBuffer();

    glutDisplayFunc(RenderSceneCB); //The function called on window redisplay, basically a while loop

    glutMainLoop();

    return 0;
}