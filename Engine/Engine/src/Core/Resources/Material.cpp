#include <fstream>
#include <iomanip>

#include "Material.h"
#include "Assimp/scene.h"
#include "Assimp/material.h"
#include "Assimp/texture.h"
#include "LoadResources.h"
#include "Texture.h"
#include "TextureData.h"


namespace Resources
{
	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<Resources::Material>("Material")
		.constructor()
		.property("Shader", &Resources::Material::shader)
		.property("Ambient", &Resources::Material::ambientColor)
		.property("Diffuse", &Resources::Material::diffuseColor)
		.property("Specular", &Resources::Material::specularColor)
		.property("Shininess", &Resources::Material::shininess)
		.property("Shininess Strenght", &Resources::Material::shininessStrength)
		.property("Variants Uniform", &Resources::Material::variants)
		;
	}

	void Material::LoadMaterialFromaiMaterial(aiMaterial* mat, const std::string& directory,
		Loader::ResourcesManager* resources, std::shared_ptr<Resources::Loader::ImporterData>& importer)
	{
		ZoneScoped
		// the importer is used in this function
		importer->maxUseOfImporter++;
		textures.resize(2);

		if(!IsSkeletal)
			shader = resources->GetShader("Default");
		else
			shader = resources->GetShader("SkeletDefault");

		LoadTextureMaterial(mat, textures[0], aiTextureType_DIFFUSE, directory, resources);
		LoadTextureMaterial(mat, textures[textures.size()- 1], aiTextureType_NORMALS, directory, resources);

		if(textures.size() <= 0 && !IsSkeletal)
			shader = resources->GetShader("DefaultNoTexture");
		else if (IsSkeletal)
			shader = resources->GetShader("Skeletal");

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		diffuseColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		ambientColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		specularColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_SHININESS, shininess);
		mat->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
		materialColor = { 1.0f, 1.0f, 1.0f };
		shaderChoose = true;

		// the importer is know not used in this function
		importer->maxUseOfImporter--;
	}

	void Material::LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& texture,
					const aiTextureType& textureType, const std::string& directory, Loader::ResourcesManager* resources)
	{
		ZoneScoped
		if (mat->GetTextureCount(textureType) > 0)
		{
			aiString path;
			if (mat->GetTexture(textureType, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				resources->LoadTexture(fullPath, texture);
				if (textureType == aiTextureType_NORMALS)
				{
					if (!IsSkeletal)
						shader = resources->GetShader("NormalMapDefault");
					else
						shader = resources->GetShader("SkeletNormalMapDefault");
				}
			}
		}
		else
		{
			textures.resize(textures.size() - 1);
		}

	}

	void Material::SendMaterial()
	{
		glUniform3fv(shader->GetLocation("umat.ambientColor"), 1, ambientColor.rgb);
		glUniform3fv(shader->GetLocation("umat.diffuseColor"), 1, diffuseColor.rgb);
		glUniform3fv(shader->GetLocation("umat.specularColor"), 1, specularColor.rgb);
		glUniform1f(shader->GetLocation("umat.shininess"), shininess);
		glUniform1f(shader->GetLocation("umat.shininessStrength"), shininessStrength);

		GLuint unit{ 0 };
		// send all the texture
		for (auto i{ 0 }; i < textures.size(); i++)
		{
			switch (i)
			{
			case 0:
				if (textures[i])
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textures[0]->texture);
					unit++;
				}
				break;
			case 1:
				if (textures[i])
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, textures[1]->texture);
					unit++;
				}
				break;
			}
		}

		SendVariants(unit);
	}

	void Material::SendVariants(GLuint unitTexture)
	{
		for (auto i{ 0 }; i < variants.size(); i++)
		{

			if (variants[i].var.get_type() == rttr::type::get<bool>())
			{
				bool b{ variants[i].var.to_bool() };
				glUniform1i(shader->GetLocation(variants[i].name.c_str()), b);
			}
			else if (variants[i].var.get_type() == rttr::type::get<int>())
			{
				int in{ variants[i].var.to_int() };
				glUniform1i(shader->GetLocation(variants[i].name.c_str()), in);
			}
			else if (variants[i].var.get_type() == rttr::type::get<float>())
			{
				float f{ variants[i].var.to_float() };
				glUniform1f(shader->GetLocation(variants[i].name.c_str()), f);
			}
			else if (variants[i].var.get_type() == rttr::type::get<Core::Maths::Vec2>())
			{
				Core::Maths::Vec2 v{ variants[i].var.get_value<Core::Maths::Vec2>() };
				glUniform2fv(shader->GetLocation(variants[i].name.c_str()), 1, v.xy);
			}
			else if (variants[i].var.get_type() == rttr::type::get<Core::Maths::Vec3>())
			{
				Core::Maths::Vec3 v{ variants[i].var.get_value<Core::Maths::Vec3>() };
				glUniform3fv(shader->GetLocation(variants[i].name.c_str()), 1, v.xyz);
			}
			else if (variants[i].var.get_type() == rttr::type::get<Core::Maths::Vec4>())
			{
				Core::Maths::Vec4 v{ variants[i].var.get_value<Core::Maths::Vec4>() };
				glUniform4fv(shader->GetLocation(variants[i].name.c_str()), 1, v.xyzw);
			}
			else if (variants[i].var.get_type() == rttr::type::get<Core::Maths::Color>())
			{
				Core::Maths::Color c{ variants[i].var.get_value<Core::Maths::Color>() };
				glUniform4fv(shader->GetLocation(variants[i].name.c_str()), 1, c.rgba);
			}
			else if (variants[i].var.get_type() == rttr::type::get<std::shared_ptr<Texture>>())
			{
				std::shared_ptr<Texture> text{ variants[i].var.get_value<std::shared_ptr<Texture>>() };
				if (text)
				{
					unitTexture++;
					glActiveTexture(GL_TEXTURE0 + unitTexture);
					glBindTexture(GL_TEXTURE_2D, text->texture);
					glUniform1i(shader->GetLocation(variants[i].name.c_str()), unitTexture);
				}
			}
		}
	}

	void Material::UpdateMaterialShader(std::shared_ptr<Shader> newShader)
	{
		shader = newShader;

		variants.clear();
		InitVariantUniform();
	}

	void Material::InitVariantUniform()
	{
		GLint count;
		GLsizei length;
		GLchar name[50];
		GLint size;
		GLenum type;

		int numberOfBasicTexture{ 0 };

		// get all the uniform used of the shader
		glGetProgramiv(shader->GetProgram(), GL_ACTIVE_UNIFORMS, &count);
		for (auto i{ 0 }; i < count; i++)
		{
			glGetActiveUniform(shader->GetProgram(), (GLuint)i, 50, &length, &size, &type, name);

			// check if it's a personal uniform
			if (name[0] != 'u')
			{
				VariantUniform uni;
				uni.name = name;

				switch (type)
				{
					case GL_BOOL:
					{
						uni.var = false;
						variants.push_back(uni);
						break;
					}

					case GL_INT:
					{
						uni.var = 0;
						variants.push_back(uni);
						break;
					}

					case GL_FLOAT:
					{
						uni.var = 0.f;
						variants.push_back(uni);
						break;
					}

					case GL_UNSIGNED_INT:
					{
						uni.var = static_cast<unsigned int>(0);
						variants.push_back(uni);
						break;
					}

					case GL_FLOAT_VEC2:
					{
						uni.var = Core::Maths::Vec2();
						variants.push_back(uni);
						break;
					}

					case GL_FLOAT_VEC3:
					{
						uni.var = Core::Maths::Vec3();
						variants.push_back(uni);
						break;
					}

					case GL_FLOAT_VEC4:
					{
						if (name[0] == 'c')
							uni.var = Core::Maths::Color(1.f, 1.f, 1.f, 1.f);
						else
							uni.var = Core::Maths::Vec4();
						variants.push_back(uni);
						break;
					}

					case GL_COLOR:
					{
						uni.var = Core::Maths::Color();
						variants.push_back(uni);
						break;
					}

					case GL_SAMPLER_2D:
					{
						uni.var = std::shared_ptr<Texture>();
						variants.push_back(uni);
						break;
					}
				}
			}
			else if (std::string(name) == "uColorTexture")
				numberOfBasicTexture++;
			else if (std::string(name) == "uNormalMap")
				numberOfBasicTexture++;
		}

		textures.resize(numberOfBasicTexture);
	}

	void  Material::SaveMaterial(const std::string& pathMaterial, const std::string& shaderPath)
	{
		std::ofstream o(pathMaterial);
		if (!o.is_open())
			return;

		json save;

		json color;

		{
			save["Shader"] = shaderPath;

			Core::Maths::Color temp{ diffuseColor };
			color["r"] = temp.r;
			color["g"] = temp.g;
			color["b"] = temp.b;
			color["a"] = temp.b;
			save["Diffuse"] = color;

			temp = ambientColor;
			color["r"] = temp.r;
			color["g"] = temp.g;
			color["b"] = temp.b;
			color["a"] = temp.a;
			save["Ambient"] = color;

			temp = ambientColor;
			color["r"] = temp.r;
			color["g"] = temp.g;
			color["b"] = temp.b;
			color["a"] = temp.b;
			save["Specular"] = color;

			if (textures.size() >= 1)
				if (textures[0])
					save["BaseTexture"] = textures[0]->name;
				else
					save["BaseTexture"] = "nothing";
			if(textures.size() >= 2)
				if (textures[1])
					save["BaseTexture"] = textures[0]->name;
				else
					save["BaseTexture"] = "nothing";


			save["shininess"] = shininess;
			save["shininessS"] = shininessStrength;
		}

		save["Variant"] = json::array();
		{
			for (auto i = 0; i < variants.size();)
				save["Variant"][i++] = SaveVariants(i);
		}

		o << std::setw(4) << save << std::endl;
	}

	bool  Material::LoadMaterialFromJSON(const std::string& pathMaterial, Resources::Loader::ResourcesManager* resources)
	{
		std::ifstream load(pathMaterial);
		if (!load.is_open())
			return false;

		json data;
		load >> data;

		{
			if (resources->GetCountShader(data["Shader"]) > 0)
				shader = resources->GetShader(data["Shader"]);

			diffuseColor.r = data["Diffuse"]["r"];
			diffuseColor.g = data["Diffuse"]["g"];
			diffuseColor.b = data["Diffuse"]["b"];
			diffuseColor.a = data["Diffuse"]["a"];

			ambientColor.r = data["Ambient"]["r"];
			ambientColor.g = data["Ambient"]["g"];
			ambientColor.b = data["Ambient"]["b"];
			ambientColor.a = data["Ambient"]["a"];

			if (!data["BaseTexture"].is_null())
			{
				textures.resize(1);
				if (data["BaseTexture"] != "nothing")
					resources->LoadTexture(data["BaseTexture"], textures[0]);
				else
					textures[0] = std::make_shared<Resources::Texture>();
			}
			if (!data["NormalMap"].is_null())
			{
				textures.resize(2);
				if (data["NormalMap"] != "nothing")
					resources->LoadTexture(data["NormalMap"], textures[1]);
				else
					textures[1] = std::make_shared<Resources::Texture>();
			}

			specularColor.r = data["Specular"]["r"];
			specularColor.g = data["Specular"]["g"];
			specularColor.b = data["Specular"]["b"];
			specularColor.a = data["Specular"]["a"];

			shininess = data["shininess"];
			shininessStrength = data["shininessS"];
		}
		shaderChoose = true;

		if(shader)
			InitVariantUniform();

		for (auto i = 0; i < variants.size(); i++)
			LoadVariants(i, data["Variant"][i], resources);

		return true;
	}

	json Material::SaveVariants(const int& index)
	{
		json out;
		if (variants[index].var.get_type() == rttr::type::get<bool>())
		{
			bool b{ variants[index].var.to_bool() };
			out["Value"] = b;
		}
		else if (variants[index].var.get_type() == rttr::type::get<int>())
		{
			int in{ variants[index].var.to_int() };
			out["Value"] = in;
		}
		else if (variants[index].var.get_type() == rttr::type::get<float>())
		{
			float f{ variants[index].var.to_float() };
			out["Value"] = f;
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Vec2>())
		{
			Core::Maths::Vec2 v{ variants[index].var.get_value<Core::Maths::Vec2>() };
			json tmp;
			tmp["x"] = v.x;
			tmp["y"] = v.y;
			out["Value"] = tmp;
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Vec3>())
		{
			Core::Maths::Vec3 v{ variants[index].var.get_value<Core::Maths::Vec3>() };
			json tmp;
			tmp["x"] = v.x;
			tmp["y"] = v.y;
			tmp["z"] = v.z;
			out["Value"] = tmp;
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Vec4>())
		{
			Core::Maths::Vec4 v{ variants[index].var.get_value<Core::Maths::Vec4>() };
			json tmp;
			tmp["x"] = v.x;
			tmp["y"] = v.y;
			tmp["z"] = v.z;
			tmp["w"] = v.w;
			out["Value"] = tmp;
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Color>())
		{
			Core::Maths::Color c{ variants[index].var.get_value<Core::Maths::Color>() };
			json tmp;
			tmp["r"] = c.r;
			tmp["g"] = c.g;
			tmp["b"] = c.b;
			tmp["a"] = c.a;
			out["Value"] = tmp;
		}
		else if (variants[index].var.get_type() == rttr::type::get<std::shared_ptr<Texture>>())
		{
			std::shared_ptr<Texture> text{ variants[index].var.get_value<std::shared_ptr<Texture>>() };
			out["Value"] = text->name;
		}

		return out;
	}

	void Material::LoadVariants(const int& index, json j, Resources::Loader::ResourcesManager* resources)
	{
		if (variants[index].var.get_type() == rttr::type::get<bool>())
		{
			variants[index].var = static_cast<bool>(j["Value"]);
		}
		else if (variants[index].var.get_type() == rttr::type::get<int>())
		{
			variants[index].var = static_cast<int>(j["Value"]);
		}
		else if (variants[index].var.get_type() == rttr::type::get<float>())
		{
			variants[index].var = static_cast<float>(j["Value"]);
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Vec2>())
		{
			Core::Maths::Vec2 v{};
			v.x = j["Value"]["x"];
			v.y = j["Value"]["y"];
			variants[index].var = v;
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Vec3>())
		{
			Core::Maths::Vec3 v{};
			v.x = j["Value"]["x"];
			v.y = j["Value"]["y"];
			v.z = j["Value"]["z"];
			variants[index].var = v;
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Vec4>())
		{
			Core::Maths::Vec4 v{};
			v.x = j["Value"]["x"];
			v.y = j["Value"]["y"];
			v.z = j["Value"]["z"];
			v.w = j["Value"]["w"];
			variants[index].var = v;
		}
		else if (variants[index].var.get_type() == rttr::type::get<Core::Maths::Color>())
		{
			Core::Maths::Color c{};
			c.r = j["Value"]["r"];
			c.g = j["Value"]["g"];
			c.b = j["Value"]["b"];
			c.a = j["Value"]["a"];
			variants[index].var = c;
		}
		else if (variants[index].var.get_type() == rttr::type::get<std::shared_ptr<Texture>>())
		{
			std::shared_ptr<Texture> text{};
			resources->LoadTexture(j["Value"], text);
			variants[index].var = text;
		}
	}

	void Material::CreateDefaultMaterial(Resources::Loader::ResourcesManager* resources)
	{
		ambientColor = { 0.f, 0.f, 0.f };
		diffuseColor = { 0.64f, 0.64f, 0.64f };
		specularColor = { 0.5f, 0.5f, 0.5f };
		shininessStrength = { 0.5f };
		shininess = { 1.f };
		shader = resources->GetShader("DefaultNoTexture");
		InitVariantUniform();
		shaderChoose = true;
	}
}