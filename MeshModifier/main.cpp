#include <iostream>
#include <direct.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
Rough script to perform a quick transform to a imported mesh and re-export it.
*/

/** @brief PI constant. */
const float PI = 3.1415926535f;

glm::vec3 convertAssimpVecToGLM(aiVector3D inVec);
void processCommandLine(int argc, char **argv);
float convertDegreesToRadians(float angle);
int main(int argc, char *argv[]);


int main(int argc, char *argv[])
{
	processCommandLine(argc, argv);

	//Get Working Directory
	char workingDir[FILENAME_MAX];
	if (_getcwd(workingDir, sizeof(workingDir)) == nullptr)
	{
		std::cout << "Error getting working directory" << std::endl;
	}
	else
	{
		std::cout << "Working Directory: " << workingDir << std::endl;
	}

	std::string importFilename = workingDir + std::string("\\") + argv[1];
	std::string exportFilename = workingDir + std::string("\\") + argv[2];

	std::cout << "Generating Transform Matrix" << std::endl;
	//Generate Transform
	glm::vec3 rotateVec(
		convertDegreesToRadians((float)std::atof(argv[4])), 
		convertDegreesToRadians((float)std::atof(argv[5])),
		convertDegreesToRadians((float)std::atof(argv[6]))
	);
	glm::mat4 mat(1.0f);
	//XYZ Rotation Order
	mat = glm::rotate(mat, rotateVec.z, glm::vec3(0, 0, 1));
	mat = glm::rotate(mat, rotateVec.y, glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, rotateVec.x, glm::vec3(1, 0, 0));


	std::cout << "Beginning Import" << std::endl;
	//Import Model
	Assimp::Importer* importer = new Assimp::Importer();
	//Read in the model file and apply Post processing flags
	int flags = (
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals
		);

	const aiScene* rawModelData = importer->ReadFile(importFilename, flags);

	if (rawModelData == nullptr)
	{
		std::cout << importFilename << " import failed: " << importer->GetErrorString() << std::endl;
		return -1;
	}

	if (rawModelData->HasMeshes())
	{
		std::cout << "Transforming Models" << std::endl;
		for (unsigned int curMesh = 0; curMesh < rawModelData->mNumMeshes; curMesh++)
		{
			aiMesh* mesh = rawModelData->mMeshes[curMesh];
			
			for (unsigned int curVertex = 0; curVertex < mesh->mNumVertices; curVertex++)
			{
				aiVector3D assimpVec = mesh->mVertices[curVertex];

				glm::vec4 v = glm::vec4(convertAssimpVecToGLM(assimpVec), 1.0f);
				v = v * mat;
				assimpVec = aiVector3D(v.x, v.y, v.z);

				mesh->mVertices[curVertex] = assimpVec;
			}
			
		}
	}
	else
	{
		std::cout << "Imported file contains no meshes" << std::endl;
		return -1;
	}

	std::cout << "Beginning Export" << std::endl;
	Assimp::Exporter* exporter = new Assimp::Exporter();

	size_t count = exporter->GetExportFormatCount();
	for (size_t i = 0; i < count; i++)
	{
		const aiExportFormatDesc* format = exporter->GetExportFormatDescription(i);
		std::cout << "ID: " << format->id << " Ext: ." << format->fileExtension <<
			" Description: " << format->description << std::endl;
	}

	if (exporter->Export(rawModelData, argv[3], exportFilename) == AI_FAILURE)
	{
		std::cout << "Export Failed, Details: " << exporter->GetErrorString() << std::endl;
		return -1;
	}

	std::cout << "Conversion Completed" << std::endl;

	importer->FreeScene();
	delete importer;
	delete exporter;
	return 0;
}

glm::vec3 convertAssimpVecToGLM(aiVector3D inVec)
{
	return glm::vec3(inVec.x, inVec.y, inVec.z);
}

void processCommandLine(int argc, char **argv)
{
	std::cout << "These were the passed in args:" << std::endl;
	for (int i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}
	std::cout << std::endl;
}

float convertDegreesToRadians(float angle)
{
	return (angle * PI / 180.0f);
}