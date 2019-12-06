#include "GrvtPch.h"
#include "nativefiledialog/nfd.h"

namespace Grvt
{

	bool OpenFile(Gfl::String& Buffer, const char* Path) {
		FILE* stream = nullptr;
		fpos_t	pos;

		stream = fopen(Path, "rb");

		if (!stream)
			return false;

		fseek(stream, 0, SEEK_END);
		fgetpos(stream, &pos);

		rewind(stream);
		Buffer.Reserve((size_t)(pos + 1));
		fread(Buffer.First(), sizeof(char), pos, stream);
		fclose(stream);

		Buffer[pos] = '\0';

		return true;
	}


	//bool DoesFileExist(const std::string& Path) {
	//	struct stat buffer;
	//	return (stat(Path.c_str(), &buffer) == 0);
	//}


	//bool GetFileNameFromPath(const char* Path, std::string& File, std::string& Directory) {
	//	const char backSlash = '\\';
	//	const char forwardSlash = '/';
	//	size_t lastIdx = -1;
	//	size_t length = strlen(Path);

	//	for (size_t i = length - 1; i >= 0; i--) {
	//		// Get first back slash or forward slash.
	//		if (Path[i] == backSlash || Path[i] == forwardSlash) {
	//			lastIdx = i;
	//			break;
	//		}
	//	}

	//	if (lastIdx == -1 || lastIdx == length - 1)
	//		return false;

	//	Iterator<char> it = Iterator<char>(&Path[0]);

	//	// Set the directory name.
	//	Directory.Append(it, lastIdx + 1);
	//	Directory[lastIdx + 1] = '\0';

	//	// Set the file name.
	//	it = it + lastIdx + 1;
	//	size_t length = Path.length() - lastIdx;
	//	File.Append(it, length);
	//	File[length - 1] = '\0';

	//	return true;
	//}


	bool OpenNativeFileDialog(WindowNativeDialogMode Mode, Gfl::String& Buffer, const char* Format) {
		nfdchar_t* outPath = NULL;
		nfdresult_t result;

		if (Mode == GrvtNativeFileDialog_OpenFile) {
			if (Format) {
				result = NFD_OpenDialog(Format, __ROOT__, &outPath);
			}
		}
		else {
			result = NFD_PickFolder(__ROOT__, &outPath);
		}

		if (result == NFD_OKAY) {
			Buffer = outPath;
			puts("Success!");
			puts(outPath);
			free(outPath);
		}
		else if (result == NFD_CANCEL) {
			puts("User pressed cancel.");
		}
		else {
			return false;
		}

		return true;
	}


	bool OpenNativeFileDialog(Gfl::Array<Gfl::String>& Buffer, const char* Format) {
		nfdpathset_t pathSet;
		nfdresult_t result = NFD_OpenDialogMultiple(Format, __ROOT__, &pathSet);

		if (result == NFD_OKAY) {
			size_t i;
			for (i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i) {
				nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);
				Buffer.Push(path);
			}
			NFD_PathSet_Free(&pathSet);
		}
		else if (result == NFD_CANCEL) {
			puts("User pressed cancel.");
		}
		else {
			return false;
		}

		return true;
	}


	void AssimpImportModelFromPath(const Gfl::String& Path, GrvtModel* Model) {
		Assimp::Importer importFile;

		GrvtMesh* mesh = nullptr;
		aiMesh* assimpMesh = nullptr;

		uint32 flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
		//if (FlipUV)
		//	flags |= aiProcess_FlipUVs;

		const aiScene* assimpScene = importFile.ReadFile(Path.C_Str(), flags);

		for (uint32 i = 0; i < assimpScene->mNumMeshes; i++) {
			assimpMesh = assimpScene->mMeshes[i];
			mesh = &Model->Meshes.Insert(GrvtMesh());

			mesh->Positions.Reserve(assimpMesh->mNumVertices, false);
			mesh->Normals.Reserve(assimpMesh->mNumVertices, false);

			if (assimpMesh->mNumUVComponents) {
				mesh->Uv.Reserve(assimpMesh->mNumVertices, false);
				mesh->Tangents.Reserve(assimpMesh->mNumVertices, false);
				mesh->Bitangents.Reserve(assimpMesh->mNumVertices, false);
			}

			glm::vec3 vector;
			glm::vec2 uv;

			for (uint32 i = 0; i < assimpMesh->mNumVertices; i++) {
				vector.x = assimpMesh->mVertices[i].x;
				vector.y = assimpMesh->mVertices[i].y;
				vector.z = assimpMesh->mVertices[i].z;

				mesh->Positions.Push(vector);

				if (assimpMesh->mNormals) {
					vector.x = assimpMesh->mNormals[i].x;
					vector.y = assimpMesh->mNormals[i].y;
					vector.z = assimpMesh->mNormals[i].z;

					mesh->Normals.Push(vector);
				}

				if (assimpMesh->mTextureCoords[0]) {
					uv.x = assimpMesh->mTextureCoords[0][i].x;
					uv.y = assimpMesh->mTextureCoords[0][i].y;

					mesh->Uv.Push(uv);
				}

				if (assimpMesh->mTangents) {
					vector.x = assimpMesh->mTangents[i].x;
					vector.y = assimpMesh->mTangents[i].y;
					vector.z = assimpMesh->mTangents[i].z;

					mesh->Tangents.Push(vector);

					vector.x = assimpMesh->mBitangents[i].x;
					vector.y = assimpMesh->mBitangents[i].y;
					vector.z = assimpMesh->mBitangents[i].z;

					mesh->Bitangents.Push(vector);
				}
			}

			for (uint32 i = 0; i < assimpMesh->mNumFaces; i++) {
				aiFace assimpFace = assimpMesh->mFaces[i];

				for (uint32 j = 0; j < assimpFace.mNumIndices; j++)
					mesh->Indices.Push(assimpFace.mIndices[j]);
			}
		}
	}

}