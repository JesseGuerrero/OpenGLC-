#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev/ogldev_math_3d.h"

GLuint VBO, VBO2, VAO;

static void CreateVertexBuffer()
{
    Vector3f Vertices[1];
    Vector3f Vertices2[1];
    Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);
    Vertices2[0] = Vector3f(0.3f, 0.0f, 0.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //We generated 3 buffers from one VBO
    glGenBuffers(1, &VBO2); //We generated 3 buffers from one VBO

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); //Stores data in some fashion
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW); //Stores data in some fashion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //Tells how to interpret
    glEnableVertexAttribArray(0);
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);

    glDrawArrays(GL_POINTS, 0, 1);

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

    int x = 100;
    int y = 100;
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("Tutorial 02");
    printf("window id: %d\n", win);

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    CreateVertexBuffer();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}