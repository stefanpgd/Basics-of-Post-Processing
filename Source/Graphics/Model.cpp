#include "Graphics/Model.h"
#include "Graphics/Mesh.h"
#include "Graphics/ShaderProgram.h"

#include "Engine/Transform.h"
#include "Engine/Camera.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cassert>

Model::Model(const std::string& path, Transform* transform)
{
	this->transform = transform;
	this->filePath = "Assets/Models/" + path;

	for(int i = 0; i < ModelArchive.size(); i++)
	{
		// Check if a model has been loaded in before, if so
		// then copy the already loaded in mesh data.
		if(filePath == ModelArchive[i]->filePath)
		{
			meshes = ModelArchive[i]->meshes;
			return;
		}
	}

	Assimp::Importer import;
	unsigned int processFlags =
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		//aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		aiProcess_SortByPType | // ?
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		//aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		//iProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_PreTransformVertices | //-- fixes the transformation issue.
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		0;

	const aiScene* scene = import.ReadFile(filePath, processFlags);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("Assimp Error: %s", import.GetErrorString());
		assert(false && "Failed to import model");
	}

	ProcessNode(scene->mRootNode, scene);

	ModelArchive.push_back(this);
}

void Model::Draw(ShaderProgram* shaderProgram)
{
	shaderProgram->SetMat4("ModelMatrix", transform->GetModelMatrix());

	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw();
	}
}

void Model::Draw(ShaderProgram* shaderProgram, Camera* camera)
{
	shaderProgram->SetMat4("ModelMatrix", transform->GetModelMatrix());
	shaderProgram->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());

	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw();
	}
}

std::string& Model::GetFilePath()
{
	return filePath;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* meshData = scene->mMeshes[node->mMeshes[i]];

		Mesh* newMesh = new Mesh(meshData, scene);
		meshes.push_back(newMesh);
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}