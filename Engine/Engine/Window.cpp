#include <glad/glad.h>
#include <stdio.h>

#include "Window.h"
#include "Renderer.h"
#include "RootObject.hpp"
#include "ComponentUpdatable.h"
#include "Loadresources.h"

static const char* gVertexShaderStr = R"GLSL(
// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

// Uniforms
uniform mat4 uModel;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;

void main()
{
    vUV = aUV;
    gl_Position = uModel * vec4(aPosition, 1.0f);
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
    //oColor = texture(uColorTexture, vUV);
	oColor = vec4(vUV,0.0f,0.0f);
})GLSL";

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
	Renderer r;
	Core::Datastructure::RootObject* root{ Core::Datastructure::RootObject::CreateRootNode() };
	Core::Datastructure::Object* o{ root->CreateChild({}) };
	Mesh* m{ new Mesh() };
	m->m_program = r.CreateProgram(gVertexShaderStr, gFragmentShaderStr);
	Resources::Loader::LoadResourcesIRenderable(m, "Resources/Dog/12228_Dog_v1_L2.obj");
	o->AddComponent(m);
	o->SetScale(Core::Maths::Vec3(.5f, .5f, .5f));
	o->SetPos({ 0, 0.f, 15.f });
	//o->SetRot({ 0, 90.f, 0.f });
	r.AddMesh(m);
	while (!glfwWindowShouldClose(m_window))
	{
		//o->Translate({ 0, 0, 0.1f });
		o->Rotate({ 0, 0.1f, 0 });
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		root->StartFrame();
		root->Update(0.2f);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		root->Render();
	}
	root->Destroy();
	root->RemoveDestroyed();
}