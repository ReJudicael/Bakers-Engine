#include "Assimp/scene.h"

#include "PxRigidStatic.h"

#include "Object3DGraph.h"
#include "LoadResources.h"
#include "RootObject.hpp"
#include "Object.hpp"
#include "EngineCore.h"
#include "PhysicsScene.h"
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
			nameMesh = directory + scene->mMeshes[node->mMeshes[0]]->mName.data;
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

		for (unsigned int i{ 1 }; i < node->mNumMeshes; i++)
		{
			Node child;
			child.position = { 0.0f,  0.0f,  0.0f };
			child.rotation = { 0.0f,  0.0f,  0.0f };
			child.scale = { 1.f, 1.f, 1.f };
			child.nameMesh = directory + scene->mMeshes[node->mMeshes[i]]->mName.data;
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
		Object->SetName(nameObject);

		Object->SetScale(scale);
		Object->SetPos(position);
		Object->SetRot(rotation);

		if (nameMesh.find("nothing") == std::string::npos)
		{
			Mesh* mesh = { new Mesh() };

			if (resources.GetCountModel(nameMesh) > 0)
			{
				//mesh->AddModel(resources.GetModel(nameMesh));
				mesh->SetChildModel(nameMesh);
				mesh->AddMaterials(resources, namesMaterial);
				Object->AddComponent(mesh);

				/*Core::Datastructure::TriggeredEvent* newEvent{ new Core::Datastructure::TriggeredEvent() };

				newEvent->SetTriggeredEvent(std::bind(&Mesh::IsModelLoaded, mesh), std::bind(&Mesh::CreateAABBMesh, mesh));

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

	void Node::SingleMeshSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory, std::vector<std::string>& materialsNam)
	{
		nameObject = node->mName.data;

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

		for (unsigned int i{ 0 }; i < currNode->mNumMeshes; i++)
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

	void Object3DGraph::SceneLoad(std::shared_ptr<Resources::Loader::ImporterData>& importer, const aiScene* scene, const aiNode* node, const std::string& directory, const bool isSingleMesh)
	{
		singleMesh = isSingleMesh;

		importer->maxUseOfImporter++;

		if (singleMesh)
			rootNodeScene.SingleMeshSceneLoad(scene, node, directory, materialsName);
		else
			rootNodeScene.RecursiveSceneLoad(scene, node, directory, materialsName);
		importer->maxUseOfImporter--;
	}

	void Object3DGraph::CreateScene(const std::string& keyName, Loader::ResourcesManager& resources, Core::Datastructure::Object* rootObject)
	{

		if (resources.GetCountScene(keyName) <= 0)
			return;
		std::shared_ptr<Object3DGraph> scene = resources.GetScene(keyName);

		scene->rootNodeScene.CreateObjectScene(rootObject, resources);
	}
}