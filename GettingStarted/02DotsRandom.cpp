#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev/ogldev_math_3d.h"

GLuint VBO;
const int numDots = 700;

static void CreateVertexBuffer()
{
    Vector3f Vertices[numDots];
    for(int i = 0; i < numDots; i++)
        Vertices[i] = Vector3f(((float) rand()/RAND_MAX)*2.0f - 1.0f, ((float) rand()/RAND_MAX) *2.0f - 1.0f, 0.0f);


    glGenBuffers(numDots, &VBO); //We generated 3 buffers from one VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); //Stores data in some fashion
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //Tells how to interpret

    glEnableVertexAttribArray(0);

    glDrawArrays(GL_POINTS, 0, numDots);

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