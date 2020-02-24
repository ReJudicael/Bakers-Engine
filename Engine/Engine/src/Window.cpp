#include <glad/glad.h>
#include <stdio.h>

#include "Window.h"
#include "Renderer.h"
#include "RootObject.hpp"
#include "ComponentUpdatable.h"
#include "Loadresources.h"
#include "PlayerCamera.h"

static const char* gVertexShaderStr = R"GLSL(
// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 Normal;

void main()
{
	vUV = aUV;
    gl_Position = uProj * uCam * uModel * vec4(aPosition, 1.0);
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


Window::Window() :
	m_width{ 1280 },
	m_height{ 800 }
{
	Init(m_width, m_height);
}

Window::Window(const int width, const int height) :
	m_width{ width },
	m_height{ height }
{
	Init(m_width, m_height);
}

void Window::Init(const int width, const int height)
{
	if (!glfwInit())
		return;

	glfwDefaultWindowHints();
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, "Default window", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	if (!gladLoadGL())
	{
		fprintf(stderr, "gladLoadGL failed. \n");
		return;
	}

	glfwSetWindowUserPointer(m_window, this);
	SetCallbackToGLFW();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void	Window::Update()
{
	m_root = Core::Datastructure::RootObject::CreateRootNode();
	Core::Datastructure::Object* camNode{ m_root->CreateChild({}) };

	PlayerCamera* c = new PlayerCamera(1200.f / 700.f, 60, 0.1, 100);
	camNode->AddComponent(c);
	Renderer r;

	Core::Datastructure::Object* o{ m_root->CreateChild({}) };
	Mesh* m{ new Mesh() };
	m->m_program = r.CreateProgram(gVertexShaderStr, gFragmentShaderStr);
	Resources::Loader::LoadResourcesIRenderable(m, "Resources/Dog/12228_Dog_v1_L2.obj");
	o->AddComponent(m);
	o->SetScale(Core::Maths::Vec3(.1f, .1f, .1f));
	o->SetPos({ 0, -2.f, -5.f });
	o->SetRot({ -90, 0.f, 0.f });
	m->SendProjectionMatrix(c->GetPerspectiveMatrix());
	r.AddMesh(m);
	while (!glfwWindowShouldClose(m_window))
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		m_root->StartFrame();
		m_root->Update(0.2f);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_root->Render();
	}
	m_root->Destroy();
	m_root->RemoveDestroyed();
}

void Window::SetCallbackToGLFW()
{
	SetKeyCallBackToGLFW();
	SetMouseButtonCallBackToGLFW();
	SetScrollCallBackToGLFW();
	SetWindowSizeToGLFW();
}

GLFWkeyfun Window::SetKeyCallBackToGLFW()
{
	GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			if (action == GLFW_PRESS)
				this_window->OnPressKey(static_cast<EKey>(key));
			if (action == GLFW_RELEASE)
				this_window->OnReleaseKey(static_cast<EKey>(key));
		}
	};
	return glfwSetKeyCallback(m_window, key_callback);
}

GLFWmousebuttonfun Window::SetMouseButtonCallBackToGLFW()
{
	GLFWmousebuttonfun mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			if (action == GLFW_PRESS)
				this_window->OnPressMouseButton(static_cast<EMouseButton>(button));
			else if (action == GLFW_RELEASE)
				this_window->OnReleaseMouseButton(static_cast<EMouseButton>(button));
		}
	};
	return glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}

GLFWscrollfun Window::SetScrollCallBackToGLFW()
{
	GLFWscrollfun scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			this_window->OnScrollYAxis(yoffset);
		}
	};
	return glfwSetScrollCallback(m_window, scroll_callback);
}

GLFWwindowsizefun Window::SetWindowSizeToGLFW()
{
	GLFWwindowsizefun window_size_callback = [](GLFWwindow* window, int width, int height)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			this_window->m_width = width;
			this_window->m_height = height;
			this_window->m_root->SetCamerasRatio((float)width / (float)height);
		}
	};
	return glfwSetWindowSizeCallback(m_window, window_size_callback);
}
