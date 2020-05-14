#include "Assimp/scene.h"

#include "PxRigidStatic.h"

#include "Object3DGraph.h"
#include "LoadResources.h"
#include "RootObject.hpp"
#include "Object.hpp"
#include "EngineCore.h"
#include "PhysicsScene.h"
#include "PxRigidStatic.h"
#include "SkeletalMesh.h"
#include "TriggeredEvent.h"
#include "Transform.hpp"
#include "RootObject.hpp"
#include "Model.h"
//#include "StaticMesh.h"

namespace Resources
{
	void Node::RecursiveSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory, std::vector<std::string>& materialsNam)
	{
		aiVector3D pos;
		aiVector3D rot;
		aiVector3D sca;

		aiMaterial* mat;
		nameObject = node->mName.data;

		if (node->mNumMeshes > 0)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
			isSkeletal = mesh->HasBones();

			nameMesh = directory + mesh->mName.data;
			mat = scene->mMaterials[scene->mMeshes[node->mMeshes[0]]->mMaterialIndex];
			namesMaterial.push_back(directory + mat->GetName().data);
			materialsNam.push_back(mat->GetName().data);

			LoadMeshsAsChild(scene, node, mat, directory, materialsNam);
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
			children[i].RecursiveSceneLoad(scene, node->mChildren[i - numCurrentChildren], directory, materialsNam);
		}
	}

	void Node::LoadMeshsAsChild(const aiScene* scene, const aiNode* node, aiMaterial* mat, const std::string& directory, std::vector<std::string>& materialsNam)
	{
		int sameKey{ 0 };
		aiMesh* mesh;
		for (unsigned int i{ 1 }; i < node->mNumMeshes; i++)
		{
			Node child;
			child.position = { 0.0f,  0.0f,  0.0f };
			child.rotation = { 0.0f,  0.0f,  0.0f };
			child.scale = { 1.f, 1.f, 1.f };
			mesh = scene->mMeshes[node->mMeshes[i]];
			isSkeletal = mesh->HasBones();
			child.nameMesh = directory + mesh->mName.data;
			child.nameObject = node->mName.data;
			mat = scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];
			std::string nameMaterial = directory + mat->GetName().data;
			if (static_cast<std::string>(scene->mMeshes[node->mMeshes[0]]->mName.data) == static_cast<std::string>(scene->mMeshes[node->mMeshes[i]]->mName.data))
			{
				sameKey++;
				child.nameMesh += std::to_string(sameKey);
				child.nameObject += std::to_string(sameKey);
				nameMaterial += std::to_string(sameKey);
			}

			child.namesMaterial.push_back(nameMaterial);
			materialsNam.push_back(nameMaterial);
			children.push_back(child);
		}
	}

	void Node::CreateObjectScene(Core::Datastructure::Object* Object, Loader::ResourcesManager& resources)
	{
		if (nameMesh.find("nothing") == std::string::npos)
		{
			if (resources.GetCountModel(nameMesh) > 0)
			{
				Mesh* mesh = new Mesh();

				if (!isSkeletal)
					mesh = new Mesh();
				else
				{
					Core::Animation::SkeletalMesh* skeletal = new Core::Animation::SkeletalMesh();
					if (resources.GetCountSkeleton(nameMesh) > 0)
					{
						skeletal->initBone(resources.GetSkeleton(nameMesh));
						skeletal->m_testAnimation = resources.m_animations.begin()->second;
					}
					mesh = skeletal;
				}

				//mesh->AddModel(resources.GetModel(nameMesh));
				mesh->SetChildModel(nameMesh);
				mesh->AddMaterials(resources, namesMaterial);
				Object->AddComponent(mesh);

				/*Core::Datastructure::TriggeredEvent* newEvent{ new Core::Datastructure::TriggeredEvent() };

				nameObject = currNode->mName.data;

				Object->AddComponent(newEvent);*/

				/*For Test*/
				//Core::Physics::StaticMesh* staticMesh{ new Core::Physics::StaticMesh() };
				//Object->AddComponent(staticMesh);

			}
		}

		for (auto i{ 0 }; i < children.size(); i++)
		{
			Core::Datastructure::Object* childObject{ Object->CreateChild("", {}) };
			children[i].CreateObjectScene(childObject, resources);
		}
	}

	void Resources::Node::SingleMeshSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory, std::vector<std::string>& materialsNam)
	{
		aiVector3D pos;
		aiVector3D rot;
		aiVector3D sca;

		node->mTransformation.Decompose(sca, rot, pos);

		position = { pos.x, pos.y, pos.z };
		rotation = { rot.x, rot.y, rot.z };
		scale = { sca.x, sca.y, sca.z };

		const aiNode* currNode = FindNodeWithMeshes(scene, node);
		nameObject = scene->mMeshes[currNode->mMeshes[0]]->mName.data;

		nameMesh = directory + scene->mMeshes[currNode->mMeshes[0]]->mName.data;

		aiMaterial* mat;
		for (unsigned int i{ 0 }; i < scene->mNumMeshes; i++)
		{
			if (!scene->HasMaterials())
			{
				namesMaterial.push_back("Default");
				materialsNam.push_back("Default");
			}
			else
			{
				mat = scene->mMaterials[scene->mMeshes[currNode->mMeshes[i]]->mMaterialIndex];
				namesMaterial.push_back(directory + mat->GetName().data);
				materialsNam.push_back(mat->GetName().data);
			}
		}
	}

	const aiNode* Node::FindNodeWithMeshes(const aiScene* scene, const aiNode* node)
	{
		if (node->mNumMeshes > 0)
			return node;
		for (int i{ 0 }; i < node->mNumChildren; i++)
		{
			const aiNode* nodeWithMesh = FindNodeWithMeshes(scene, node->mChildren[i]);
			if (nodeWithMesh != nullptr)
				return nodeWithMesh;
		}
		return nullptr;
	}

	void Object3DGraph::SceneLoad(std::shared_ptr<Resources::Loader::ImporterData>& importer, const aiScene* scene, const aiNode* node, const std::string& directory, const bool isSingleMesh)
	{
		importer->maxUseOfImporter++;

		if (singleMesh)
			rootNodeScene.SingleMeshSceneLoad(scene, node, directory, materialsName);
		else
			rootNodeScene.RecursiveSceneLoad(scene, node, directory, materialsName);
		importer->maxUseOfImporter--;
	}


	void Object3DGraph::CreateScene(const std::string& keyName, 
									Resources::Loader::ResourcesManager& resources, 
									Core::Datastructure::Object* rootObject)
	{
		if (resources.GetCountScene(keyName) <= 0)
			return;
		std::shared_ptr<Object3DGraph> scene = resources.GetScene(keyName);

		Core::Datastructure::Object* object = rootObject;

		/*if (!scene->singleMesh)
		{
			auto index = keyName.find_last_of("/");
			std::string name;
			name = keyName.substr(index + 1, keyName.size() - 1);

			index = name.find_last_of(".");
			name = name.substr(0, index);

			object = rootObject->CreateChild(name, {});
		}*/

		scene->rootNodeScene.CreateObjectScene(object, resources);
	}
}