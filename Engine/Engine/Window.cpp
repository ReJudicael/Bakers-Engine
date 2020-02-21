#include <glad/glad.h>
#include <stdio.h>

#include "Window.h"
#include "Renderer.h"
#include "Object.hpp"
#include "Camera.h"

Window::Window()
{
	if (!glfwInit())
		return;

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	m_window = glfwCreateWindow(1280, 720, "Default window", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	if (!gladLoadGL())
	{
		fprintf(stderr, "gladLoadGL failed. \n");
		return;
	}
}

Window::Window(const int height, const int width)
{
	if (!glfwInit())
		return;
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	m_window = glfwCreateWindow(1280, 720, "Default window", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	if (!gladLoadGL())
	{
		fprintf(stderr, "gladLoadGL failed. \n");
		return;
	}
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void	Window::Update()
{
	Core::Datastructure::Object* t{ Core::Datastructure::Object::CreateRootNode() };
	Camera* c = new Camera(1280.f / 720.f, 60, 0.1, 100);
	t->AddComponent(c);
	c->UpdateCamera();
	Renderer r;
	Core::Datastructure::Object* o{ Core::Datastructure::Object::CreateRootNode() };
	Mesh* m{ r.CreateCube() };
	o->AddComponent(m);
	m->SendProjectionMatrix(c->GetPerspectiveMatrix());
	r.AddMesh(m);
	while (!glfwWindowShouldClose(m_window))
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		r.Render();
	}
}