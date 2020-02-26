#include <glad/glad.h>
#include <stdio.h>

#include "Window.h"
#include "Renderer.h"
#include "RootObject.hpp"
#include "ComponentUpdatable.h"
#include "Loadresources.h"
#include "Camera.h"

static const char* gVertexShaderStr = R"GLSL(
// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 Normal;

void main()
{
	vUV = aUV;
    gl_Position = uProj * uModel * vec4(aPosition, 1.0);
})GLSL";

static const char* gFragmentShaderStr = R"GLSL(
// Varyings
in vec2 vUV;


// Uniforms
uniform sampler2D uColorTexture;

// Shader outputs
out vec4 oColor;

void main()
{
    oColor = texture(uColorTexture, vUV);
	//oColor = vec4(vUV,0.0f,0.0f);
})GLSL";

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
	Resources::Loader::ResourcesManager manager;
	
	//Core::Datastructure::RootObject* root{ Core::Datastructure::RootObject::CreateRootNode() };
	Core::Datastructure::Object* o{ root->CreateChild({}) };

	manager.CreateProgram(gVertexShaderStr, gFragmentShaderStr);

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