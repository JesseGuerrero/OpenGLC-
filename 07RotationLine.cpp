#include <stdio.h>
#include <string.h>

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "ogldev/ogldev_math_3d.h"
#include "jesseShader.h"

GLuint VBO;
GLint gRotationLocation;
Shader *ourShader;
const char* pVSFileName = "Shaders/RotationVertex.glsl";
const char* pFSFileName = "Shaders/RotationFragment.glsl";

static void RenderSceneCB();
static void CreateVertexBuffer();
static void CompileShaders();

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
    int win = glutCreateWindow("Tutorial 07");
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

    CompileShaders();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float AngleInRadians = 0.0f;
    static float Delta = 0.01f;

    AngleInRadians += Delta;
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, (float)AngleInRadians, glm::vec3(0.0f, 0.0f, 1.0f));


    glUniformMatrix4fv(gRotationLocation, 1, GL_TRUE, glm::value_ptr(transform));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(0);

    glutPostRedisplay();

    glutSwapBuffers();
}


static void CreateVertexBuffer()
{
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-0.5f, 0.0f, 0.0f);   // bottom left
    Vertices[1] = Vector3f(0.5f, 0.0f, 0.0f);    // bottom right
    //Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);     // top

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CompileShaders()
{
    ourShader = (Shader*) new Shader(pVSFileName, pFSFileName);
    ourShader->use();
    gRotationLocation = glGetUniformLocation(ourShader->ptrToShader, "gRotation");
    if (gRotationLocation == -1) {
        printf("Error getting uniform location of 'gRotation'\n");
        exit(1);
    }
}

