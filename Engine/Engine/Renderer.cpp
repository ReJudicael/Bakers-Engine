#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>
#include <iostream>

#include "Renderer.h"
#include "Vec4.hpp"
#include "Vec2.hpp"

static const char* gVertexShaderStr = R"GLSL(
// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

// Uniforms
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;

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

	std::cout << "program load" << std::endl;
	return Program;
}

GLuint Renderer::CreateTextureFromColor(const Core::Maths::Vec4& color)
{
	GLuint texture;

	std::vector<Core::Maths::Vec4> Texels(5 * 5);
	for (int i = 0, j = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
			Texels[j + i * 5] = color;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 5, 5, 0, GL_RGBA, GL_FLOAT, &Texels[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return texture;
}

GLuint Renderer::CreateTextureFromImage(const char* filename, bool shouldFlip)
{
	std::string s = filename;
	GLuint		texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(shouldFlip);
	unsigned char* data = stbi_load(s.c_str(), &width, &height, &nrChannels, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load " << filename << std::endl;
	}

	stbi_image_free(data);

	return texture;
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
	m->Start();
	m->m_program = CreateProgram(gVertexShaderStr, gFragmentShaderStr);
	m->m_vertexCount = 6;

	float Plane[] = {
		-1.f, -1.f, 0.f, -1.f, -1.f,
		1.f, -1.f, 0.f, 1.f, -1.f,
		-1.f, 1.f, 0.f, -1.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 1.f
	};

	unsigned int Indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	glGenVertexArrays(1, &m->m_VAO);
	glBindVertexArray(m->m_VAO);

	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Plane), Plane, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	m->m_texture = CreateTextureFromColor(Core::Maths::Vec4{ 1.f, 0.f, 0.f, 1.f });
	//m->m_texture = CreateTextureFromImage("Resources/Textures/block.png");
	
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, UV));
	glBindVertexArray(0);

	return m;
}

Mesh* Renderer::CreateCube()
{
	Mesh* m = new Mesh();
	//m->Start();
	m->m_program = CreateProgram(gVertexShaderStr, gFragmentShaderStr);
	m->m_vertexCount = 36;
	

	float Cube[] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f
	};

	unsigned int Indices[] = {
		0, 1, 2,
		1, 2, 3,
		3, 7, 1,
		1, 7, 5,
		7, 5, 4,
		4, 7, 6,
		2, 6, 0, 
		0, 6, 4,
		1, 6, 7,
		3, 6, 7,
		0, 1, 4, 
		4, 1, 5
	};

	glGenVertexArrays(1, &m->m_VAO);
	glBindVertexArray(m->m_VAO);
	
	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube), Cube, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	m->m_texture = CreateTextureFromColor(Core::Maths::Vec4{ 1.f, 1.f, 0.f, 1.f });
	//m->m_texture = CreateTextureFromImage("Resources/Textures/block.png");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, UV));
	glBindVertexArray(0);

	return m;
}
