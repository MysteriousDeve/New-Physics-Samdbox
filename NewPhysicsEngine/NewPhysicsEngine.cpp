#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <math.h>
#include <glm/glm.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Physics/Scene.h"
#include "Physics/Entity.h"
#include "Physics/Transform2D.h"
#include "UI/InteractionHandler.h"

using namespace std;
using namespace glm;


int w = 1400, h = 800;
void ErrorCallback(int errorCode, const char* description);
void ResizeCallback(GLFWwindow* window, int w, int h);
void InitializeWindow(GLFWwindow* window);
void Panning(GLFWwindow* window);


Camera cam(Vector2(0.0f, 0.0f), 1, 0, w / float(h));

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    glfwSetErrorCallback(ErrorCallback);
    glfwInit();

    GLFWwindow* win = glfwCreateWindow(w, h, "My Physics Engine", NULL, NULL);

    if (win == NULL)
    {
        glfwTerminate();
        return -1;
    }
    InitializeWindow(win);
    
    gladLoadGL();
    glViewport(0, 0, w, h);


    GLfloat vert[]{
        /*-1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,*/
         -0.5f, -0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
          0.5f, -0.5f, 0.0f
    };
    GLuint index[]{
        0, 1, 2, 2, 3, 0
    };
    Renderer renderer(
        MeshInfo(vert, sizeof(vert), index, sizeof(index)),
        "Common", "Circle"
    );
    renderer.Initialize();
    
    Scene scene;
    scene.InitializeSceneTesting(30);
    scene.NarrowCollisionsTest();

    InteractionHandler handler(win, &cam);

    while (!glfwWindowShouldClose(win))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        

        Panning(win);
        renderer.SetCameraTransformation(cam.GetCameraMatrix(), cam.GetPosition());
        renderer.Render(
            scene.entities.size(), 
            [&scene](int i, Shader* shader) 
            {
                scene.RenderScene(i, shader); 
            }
        );

        Vector2 mPos = handler.GetMouseWorldPos();
        renderer.Render(
            1,
            [&mPos](int i, Shader* shader)
            {
                shader->SetUniformVec4("Color", 1, 1, 1, 0);
                shader->SetUniformVec2("Position", mPos.x, mPos.y);
                shader->SetUniformVec2("Size", 0.1, 0.1);
            }
        );

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    renderer.Terminate();
    glfwTerminate();
    return 0;
}

void InitializeWindow(GLFWwindow* window)
{
    glfwSetWindowSizeCallback(window, ResizeCallback);
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
}

void ErrorCallback(int errorCode, const char* description)
{
    MessageBoxA(NULL, description, "Error", MB_ICONERROR | MB_OK);
}

void ResizeCallback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
    cam.SetWHratio(w / float(h));
}

void Panning(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cam.Translate(Vector2(-1 / 60.0, 0));
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cam.Translate(Vector2(1 / 60.0, 0));
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cam.Translate(Vector2(0, 1 / 60.0));
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cam.Translate(Vector2(0, -1 / 60.0));
    }
}