#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "GUIManager.h"
#include "stb_image.h"
#include <stdio.h>
#include <iostream>
#include <WindowFileBrowser.h>
#include <WindowConsole.h>
#include <WindowInspector.h>
#include <WindowScene.h>
#include <WindowHierarchy.h>
#include <WindowProfiler.h>
#include <MenuGroup.h>

int main()
{
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Editor", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL())
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    GLFWimage icons[1];
    icons[0].pixels = stbi_load("Resources\\Images\\bread.png", &icons[0].width, &icons[0].height, 0, STBI_rgb_alpha);
    glfwSetWindowIcon(window, 1, icons);
    stbi_image_free(icons[0].pixels);

    // Our state
    bool show_demo_window = true;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.6f, 1.f);

    Editor::GUIManager* man = new Editor::GUIManager(window, glsl_version, Editor::GUIStyle::BAKER);

    Editor::Canvas* canvas = new Editor::Canvas();

    canvas->Add<Editor::Window::WindowHierarchy>();
    canvas->Add<Editor::Window::WindowInspector>();
    canvas->Add<Editor::Window::WindowScene>();
    canvas->Add<Editor::Window::WindowConsole>();
    canvas->Add<Editor::Window::WindowFileBrowser>();
    canvas->Add<Editor::Window::WindowProfiler>(false);

    man->SetCanvas(canvas);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        // Start the Dear ImGui frame

        man->Render();
        // Rendering

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
    }
    delete man;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
