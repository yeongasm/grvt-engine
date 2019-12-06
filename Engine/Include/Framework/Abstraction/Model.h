#pragma once

#ifndef GRAVITY_ABSTRACTION_MODEL
#define GRAVITY_ABSTRACTION_MODEL

#include "Minimal.h"
#include "Framework/Foundation/Foundations.h"

namespace Grvt
{

	/**
	* Revamp Status: Completed!
	*/


	/**
	* Gravity's ModelImportInfo object.
	* To be supplied into the ResourceManager when importing a model into the engine.
	*
	* @param Name - Model's identity.
	* @param Path - Path to model's file.
	*/
	struct ModelImportInfo
	{
		Gfl::String	Name;
		Gfl::String	Path;
	};


	/**
	*/
	class GrvtMesh
	{
	public:

		ObjHandle			Vao;
		ObjHandle			Vbo;
		ObjHandle			Ebo;

		uint32				Size;

		Gfl::Array<glm::vec3>	Positions;
		Gfl::Array<glm::vec2>	Uv;
		Gfl::Array<glm::vec3>	Normals;
		Gfl::Array<glm::vec3>	Tangents;
		Gfl::Array<glm::vec3>	Bitangents;
		Gfl::Array<uint32>		Indices;

		GrvtMesh();
		~GrvtMesh();

		GrvtMesh(const GrvtMesh&) = delete;
		GrvtMesh& operator= (const GrvtMesh&) = delete;

		GrvtMesh(GrvtMesh&&);
		GrvtMesh& operator= (GrvtMesh&&);

	private:

		void Free();

	};


	/**
	* Revamped Model object.
	*
	* Stores the raw data of a model that can be processed for dynamic usage.
	* Models can be of an imported type or a custom type.
	*
	* CHANGELOG:
	* We removed the ModelData abstraction layer as it was redundant and make it contain all the raw information instead.
	* The reason for doing this was because this raw Model object should only be a placeholder for the data and do nothing else.
	*/
	class GrvtModel
	{
	public:

		Gfl::Array<GrvtMesh>	Meshes;

		GrvtModel();
		~GrvtModel();

	private:

		GrvtModel(const GrvtModel&) = delete;
		GrvtModel& operator= (const GrvtModel&) = delete;

		GrvtModel(GrvtModel&&) = delete;
		GrvtModel& operator= (GrvtModel&&) = delete;

	public:

		/**
		* Appends the information supplied in ModelImportInfo into it.
		* Does not however create the model in the GPU.
		*/
		GrvtModel* Alloc(const ModelImportInfo& Info);

		/**
		* Releases the information supplied into this GrvtModel object.
		* Does not however delete the model from the GPU.
		*
		* Do not rely on the destructor to release the data within this object as it does nothing.
		*/
		void		Free();

	};

}

#endif // !GRAVITY_ABSTRACTION_MODEL