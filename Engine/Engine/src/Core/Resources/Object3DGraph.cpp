#include "Assimp/scene.h"
#include "Object3DGraph.h"
#include "LoadResources.h"
#include "Object.hpp"

namespace Resources
{
	void Node::RecursiveSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory)
	{
		aiVector3D pos;
		aiVector3D rot;
		aiVector3D sca;

		aiMaterial* mat;
		nameObject = node->mName.data;

		if (node->mNumMeshes > 0)
		{
			nameMesh = directory + scene->mMeshes[node->mMeshes[0]]->mName.data;
			mat = scene->mMaterials[scene->mMeshes[node->mMeshes[0]]->mMaterialIndex];
			namesMaterial.push_back(directory + mat->GetName().data);

			for (int i{ 1 }; i < node->mNumMeshes; i++)
			{
				Node child;
				child.position = { 0.0f,  0.0f,  0.0f };
				child.rotation = { 0.0f,  0.0f,  0.0f };
				child.scale = { 1.f, 1.f, 1.f };
				child.nameMesh = directory + scene->mMeshes[node->mMeshes[i]]->mName.data + std::to_string(i);
				mat = scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];
				child.namesMaterial.push_back(directory + mat->GetName().data + std::to_string(i));
				children.push_back(child);
				//std::cout << "num Mesh in the scene " << scene->mMeshes[node->mMeshes[i]]->mName.data << " curr name Mesh   " << nameMesh << std::endl;
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
				mesh->AddMaterials(resources, namesMaterial);
				std::cout << nameObject << std::endl;
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

	void Node::SingleMeshSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory)
	{
		aiVector3D pos;
		aiVector3D rot;
		aiVector3D sca;

		node->mTransformation.Decompose(sca, rot, pos);

		position = { pos.x, pos.y, pos.z };
		rotation = { rot.x, rot.y, rot.z };
		scale = { sca.x, sca.y, sca.z };

		const aiNode* currNode = currNode = node->mChildren[0];
		aiMaterial* mat;

		nameMesh = nameMesh = directory + scene->mMeshes[currNode->mMeshes[0]]->mName.data;

		for (int i{ 0 }; i < currNode->mNumMeshes; i++)
		{
			mat = scene->mMaterials[scene->mMeshes[currNode->mMeshes[i]]->mMaterialIndex];
			namesMaterial.push_back(directory + mat->GetName().data);
		}

	}

	void Object3DGraph::SceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory, const bool isSingleMesh)
	{
		singleMesh = isSingleMesh;

		if (singleMesh)
			rootNodeScene.SingleMeshSceneLoad(scene, node, directory);
		else
			rootNodeScene.RecursiveSceneLoad(scene, node, directory);
	}

	void Object3DGraph::CreateScene(const std::string& keyName, Loader::ResourcesManager& resources, Core::Datastructure::Object* rootObject)
	{

		if (resources.GetCountScene(keyName) <= 0)
			return;
		std::shared_ptr<Object3DGraph> scene = resources.GetScene(keyName);

		scene->rootNodeScene.CreateObjectScene(rootObject, resources);
	}
}