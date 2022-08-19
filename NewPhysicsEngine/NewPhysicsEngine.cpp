#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"
#include "Physics/Scene.h"
#include "Physics/Entity.h"
#include "Physics/Geometry.h"
#include "Physics/Transform2D.h"
#include "UI/InteractionHandler.h"

using namespace std;
using namespace glm;


int w = 1400, h = 800;
bool running = true;

void ErrorCallback(int errorCode, const char* description);
void ResizeCallback(GLFWwindow* window, int w, int h);
void InitializeWindow(GLFWwindow* window);
void Panning(GLFWwindow* window);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


Camera cam(Vector2(0.0f, 0.0f), 1, 0, w / float(h));

int main()
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
    scene.InitializeSceneTesting(150);
    

    InteractionHandler handler(win, &cam);

    //Geometry e(
    //    Transform2D(
    //        Vector2(0, 0),
    //        0,
    //        Vector2(0.2, 0.2),
    //        0
    //    ),
    //    EntityType::Circle
    //);
    //e.density = 1;
    //scene.AddGeometry(e);
    //Geometry* eref = scene.GetGeometryRef(scene.geometries.size() - 1);

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(win))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Panning(win);
        renderer.SetCameraTransformation(cam.GetCameraMatrix(), cam.GetPosition());

        //eref->transform.position = handler.GetMouseWorldPos();

        float currentTime = glfwGetTime();
        float dt = currentTime - lastTime;

        if (running)
        {
            scene.Update(dt / 20.0);
        }
        lastTime = currentTime;
        
        renderer.Render(
            scene.geometries.size(),
            [&scene](int i, Shader* shader) 
            {
                scene.RenderScene(i, shader); 
            }
        );

        Vector2 mousePos = handler.GetMouseWorldPos();
        renderer.Render(
            1,
            [&mousePos](int i, Shader* shader)
            {
                shader->SetUniformVec4("Color", 1, 1, 1, 1);
                shader->SetUniformVec2("Position", mousePos.x, mousePos.y);
                shader->SetUniformVec2("Size", 0.1, 0.1);
            }
        );

        std::cout << dt;

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
    glfwSetKeyCallback(window, KeyCallback);
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

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        running = !running;
    }
}