#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev_math_3d.h"

GLuint VBO;

static void CreateVertexBuffer()
{
    Vector3f Vertices[9];
    Vertices[0] = Vector3f(-0.1f, -0.1f, 0.0f);   // bottom left
    Vertices[1] = Vector3f(0.1f, -0.1f, 0.0f);    // bottom right
    Vertices[2] = Vector3f(0.0f, 0.1f, 0.0f);     // top
    Vertices[3] = Vector3f(-0.4f, -0.1f, 0.0f);   // bottom left
    Vertices[4] = Vector3f(-0.2f, -0.1f, 0.0f);    // bottom right
    Vertices[5] = Vector3f(-0.3f, 0.1f, 0.0f);     // top
    Vertices[6] = Vector3f(0.2f, -0.1f, 0.0f);   // bottom left
    Vertices[7] = Vector3f(0.4f, -0.1f, 0.0f);    // bottom right
    Vertices[8] = Vector3f(0.3f, 0.1f, 0.0f);     // top

    glGenBuffers(3, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 9);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    int width = 800;
    int height = 600;
    glutInitWindowSize(width, height);

    int x = 200;
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