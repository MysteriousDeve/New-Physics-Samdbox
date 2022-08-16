#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <math.h>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Physics/Scene.h"

using namespace std;

int w = 1400, h = 800;

//---Function forward declaration---///////////////////////
void ErrorCallback(int errorCode, const char* description);
void ResizeCallback(GLFWwindow* window, int w, int h);
void InitializeWindow(GLFWwindow* window);
void AlignViewport(int w, int h);
///////////////////////////////////////////////////////////

Camera cam(Vector2(0.0f, 0.0f), 0.5, 0, w / float(h));

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
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
    };
    GLuint index[]{
        0, 1, 2, 2, 3, 0
    };
    Renderer renderer(
        MeshInfo(vert, sizeof(vert), index, sizeof(index)),
        "Circle", "Something"
    );
    renderer.Initialize();
    
    Scene scene;
    scene.InitializeSceneTesting(30);
    scene.NarrowCollisionsTest();

    

    while (!glfwWindowShouldClose(win))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.SetCameraMatrix(cam.GetCameraMatrix());
        renderer.Render(
            scene.entities.size(), 
            [&scene](int i, Shader* shader) 
            {
                scene.RenderScene(i, shader); 
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