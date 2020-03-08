#include "Assimp/scene.h"
#include "SceneData.h"
#include "LoadResources.h"
#include "Object.hpp"

namespace Resources
{
	void Node::RecursiveSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory)
	{
		aiVector3D pos;
		aiVector3D rot;
		aiVector3D sca;

		if (node->mNumMeshes > 0)
		{
			nameMesh = directory + scene->mMeshes[node->mMeshes[0]]->mName.data;
			// std::cout "\t \t \t new Node  " << nameMesh << std::endl;


			for (int i{ 1 }; i < node->mNumMeshes; i++)
			{
				Node child;
				child.position = { 0.0f,  0.0f,  0.0f };
				child.rotation = { 0.0f,  0.0f,  0.0f };
				child.scale = { 1.f, 1.f, 1.f };
				child.nameMesh = directory + scene->mMeshes[node->mMeshes[i]]->mName.data + std::to_string(i);
				children.push_back(child);
				// std::cout "num Mesh in the scene " << scene->mMeshes[node->mMeshes[i]]->mName.data << std::endl;
			}
		}
		else
		{
			nameMesh = "nothing";
		}

		node->mTransformation.Decompose(sca, rot, pos);

		position =	{ pos.x, pos.y, pos.z };
		rotation =	{ rot.x, rot.y, rot.z };
		scale =		{ sca.x, sca.y, sca.z };

		int numChildren{ (int)children.size() + (int)node->mNumChildren };
		int numCurrentChildren = (int)children.size();

		if (numChildren > 0)
			children.resize(numChildren);
		for (int i{ numCurrentChildren }; i < numChildren; i++)
		{
			children[i].RecursiveSceneLoad(scene, node->mChildren[i - numCurrentChildren], directory);
		}
	}

	void Node::CreateObjectScene(Core::Datastructure::Object* Object, Loader::ResourcesManager& resources)
	{
		if (nameMesh.find("nothing") == std::string::npos)
		{
			Mesh* mesh = { new Mesh() };

			if (resources.GetCountModel(nameMesh) > 0)
			{
				mesh->AddModel(resources.GetModel(nameMesh));
				mesh->m_program = resources.GetShader("Default");
				Object->AddComponent(mesh);
			}
		}

		Object->SetScale(scale);
		Object->SetPos(position);
		Object->SetRot(rotation);

		for (auto i{ 0 }; i < children.size(); i++)
		{
			Core::Datastructure::Object* childObject{ Object->CreateChild({}) };
			children[i].CreateObjectScene(childObject, resources);
		}
	}

	void SceneData::CreateScene(const std::string& keyName, Loader::ResourcesManager& resources, Core::Datastructure::Object* rootObject)
	{
		std::shared_ptr<SceneData> scene = resources.GetScene(keyName);

		scene->rootNodeScene.CreateObjectScene(rootObject, resources);
	}
}