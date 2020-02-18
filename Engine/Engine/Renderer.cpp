#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Vec4.hpp"
#include "Vec2.hpp"

static const char* gVertexShaderStr = R"GLSL(
// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

// Uniforms
uniform mat4 uModelViewProj;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;

void main()
{
    vUV = aUV;
    gl_Position = vec4(aPosition, 1.0);
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
})GLSL";


Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

GLuint	Renderer::CreateProgram(const char* vertex, const char* fragment)
{
	std::cout << "Tried to create program" << std::endl;
	GLuint Program = glCreateProgram();

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::vector<const char*> Sources;
	Sources.push_back("#version 330 core");
	Sources.push_back(vertex);
	glShaderSource(VertexShader, (GLsizei)Sources.size(), &Sources[0], nullptr);
	glCompileShader(VertexShader);
	GLint Compile;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Compile);
	if (Compile == GL_FALSE)
	{
		std::cout << "Vertex shader didn't load." << std::endl;
		return 0;
	}

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::vector<const char*> FragSources;
	FragSources.push_back("#version 330 core");
	FragSources.push_back(fragment);
	glShaderSource(FragmentShader, (GLsizei)FragSources.size(), &FragSources[0], nullptr);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Compile);
	if (Compile == GL_FALSE)
	{
		std::cout << "Fragment shader didn't load." << std::endl;
		return 0;
	}

	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);

	glLinkProgram(Program);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return Program;
}

void	Renderer::Render()
{
	{
		for (int i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->DrawMesh();
		}
	}
}

void	Renderer::AddMesh(Mesh* newMesh)
{
	m_meshes.push_back(newMesh);
}

struct vertex
{
	Core::Maths::Vec3 Position;
	Core::Maths::Vec2 UV;
};

Mesh*	Renderer::CreatePlane()
{
	Mesh* m = new Mesh();
	m->m_program = CreateProgram(gVertexShaderStr, gFragmentShaderStr);
	m->m_vertexCount = 6;

	float Plane[] = {
		-1.f, -1.f, 0.f, -1.f, -1.f,
		1.f, -1.f, 0.f, 1.f, -1.f,
		-1.f, 1.f, 0.f, -1.f, 1.f,
		1.f, -1.f, 0.f, 1.f, -1.f,
		-1.f, 1.f, 0.f, -1.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 1.f
	};

	glGenBuffers(1, &m->m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m->m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Plane), Plane, GL_STATIC_DRAW);

	std::vector<Core::Maths::Vec4> Texels(5 * 5);
	std::vector<Core::Maths::Vec4> Texels2(5 * 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Texels[j + i * 5] = Core::Maths::Vec4{ 1.f, 0.1f * i, 0.1f * j, 1.f };
			Texels2[j + i * 5] = Core::Maths::Vec4{ 0.1f * j, 0.1f * i, 1.f, 1.f };
		}
	}

	glGenTextures(1, &m->m_texture);
	glBindTexture(GL_TEXTURE_2D, m->m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 5, 5, 0, GL_RGBA, GL_FLOAT, &Texels[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindVertexArray(m->m_VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, UV));
	glBindVertexArray(0);

	return m;
}