#include <stdio.h>
#include <string.h>

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev/ogldev_math_3d.h"
#include "ogldev/jesse/jesse.h"

GLuint VBO;
GLint gRotationLocation;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float AngleInRadians = 0.0f;
    static float Delta = 0.01f;

    AngleInRadians += Delta;
    Matrix4f Rotation(cosf(AngleInRadians), -sinf(AngleInRadians), 0.0f, 0.0f,
                      sinf(AngleInRadians), cosf(AngleInRadians),  0.0f, 0.0f,
                      0.0,                  0.0f,                  1.0f, 0.0f,
                      0.0f,                 0.0f,                  0.0f, 1.0f);

    glUniformMatrix4fv(gRotationLocation, 1, GL_TRUE, &Rotation.m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glutPostRedisplay();

    glutSwapBuffers();
}


static void CreateVertexBuffer()
{
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-0.5f, -0.5f, 0.0f);   // bottom left
    Vertices[1] = Vector3f(0.5f, -0.5f, 0.0f);    // bottom right
    Vertices[2] = Vector3f(0.0f, 0.5f, 0.0f);     // top

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

const char* pVSFileName = "../Shaders/RotationVertex.glsl";
const char* pFSFileName = "../Shaders/RotationFragment.glsl";

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs = ReadFileJesse(pVSFileName);
    std::string fs = ReadFileJesse(pFSFileName);

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    gRotationLocation = glGetUniformLocation(ShaderProgram, "gRotation");
    if (gRotationLocation == -1) {
        printf("Error getting uniform location of 'gRotation'\n");
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);
}


#include <iostream>
#include <filesystem>
int main(int argc, char** argv)
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current Path: " << currentPath << std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    int width = 1920;
    int height = 1080;
    glutInitWindowSize(width, height);

    int x = 200;
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