#include <glad/glad.h>
#include <stdio.h>

#include "Window.h"
#include "Renderer.h"
#include "RootObject.hpp"
#include "ComponentUpdatable.h"
#include "Loadresources.h"
#include "Camera.h"

Window::Window()
{
	if (!glfwInit())
		return;

	glfwDefaultWindowHints();
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	Core::Datastructure::RootObject* root{ Core::Datastructure::RootObject::CreateRootNode() };
	Camera* c = new Camera(1200.f / 700.f, 60, 0.1, 100);
	root->AddComponent(c);
	c->UpdateCamera();
	Renderer r;
	Resources::Loader::ResourcesManager manager{};
	
	//Core::Datastructure::RootObject* root{ Core::Datastructure::RootObject::CreateRootNode() };
	Core::Datastructure::Object* o{ root->CreateChild({}) };

	Mesh* testMesh{ new Mesh() };
	testMesh->SendProjectionMatrix(c->GetPerspectiveMatrix());
	manager.LoadResourcesIRenderable(testMesh, "Resources/Umbreon/UmbreonHighPoly.obj", o);
	//manager.LoadResourcesIRenderable(testMesh, "Resources/level.fbx", o);

	//m->SendProjectionMatrix(c->GetPerspectiveMatrix());
	//r.AddMesh(m);
	while (!glfwWindowShouldClose(m_window))
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		manager.linkAllTextureToOpenGl();
		manager.linkAllModelToOpenGl();
		root->StartFrame();
		root->Update(0.2f);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		root->Render();
	}
	root->Destroy();
	root->RemoveDestroyed();
}