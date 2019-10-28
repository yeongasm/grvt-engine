#pragma once


/**
* Mid Level API a.k.a Middleware.
*
* The data structure and functions in this file bridges the gap between the high-level API which are foward declared above with the low-level API.
* It designed to be generic meaning only data structures that are commonly used in other objects should be passed into the low-level API.
*/


/**
* Forward declare all higher level graphic objects signature.
*/
class GrvtMesh;
class GrvtModel;
class GrvtTexture;
class GrvtShader;
class GrvtPostProcess;


namespace Middleware {


	/**
	* [MIDDLEWARE]
	* ResourcePacket data structure.
	*/
	template <class ResourceType, class BuildDataType>
	struct ResourcePacket {

		ResourceType* ResourcePtr;
		BuildDataType BuildData;

		ResourcePacket() : 
			ResourcePtr(nullptr), BuildData() {}

		~ResourcePacket() {}

		ResourcePacket(ResourceType* Pointer, BuildDataType Data) :
			ResourcePtr(Pointer), BuildData(Data) {}

	};


	/**
	* [MIDDLEWARE]
	* GfxObjectType enums.
	*
	* A list of available graphics object in the engine.
	* Used for identifying the derived class of GraphicsObject.
	*/
	enum GfxObjectType : uint32 {

		GrvtGfx_Type_None			= 0x00,	// Used only for initialisation.
		GrvtGfx_Type_MeshBuffer		= 0x01,
		GrvtGfx_Type_VertexArray	= 0x02,
		GrvtGfx_Type_Texture		= 0x03,
		GrvtGfx_Type_Shader			= 0x04,
		GrvtGfx_Type_PostProcess	= 0x05,
		GrvtGfx_Type_RenderBuffer	= 0x06

	};


	/**
	* [MIDDLEWARE]
	* DeletePacket data strucure.
	*
	* A DeletePacket is required to remove a GraphicsObject from the GPU before removing it from memory.
	*/
	struct DeletePacket {

		ObjHandle		Handle;
		GfxObjectType	Type;

		DeletePacket();
		~DeletePacket();

		DeletePacket(ObjHandle &&Resource, GfxObjectType Type);

		DeletePacket(const DeletePacket &Rhs)				= delete;
		DeletePacket& operator= (const DeletePacket &Rhs)	= delete;

		DeletePacket(DeletePacket &&Rhs);
		DeletePacket& operator= (DeletePacket &&Rhs);

	};


	using MeshPacket		= ResourcePacket<GrvtMesh,			BaseAPI::MeshBuildData>;
	using TexturePacket		= ResourcePacket<GrvtTexture,		BaseAPI::TextureBuildData>;
	using ShaderPacket		= ResourcePacket<GrvtShader,		BaseAPI::ShaderBuildData>;
	using FramebufferPacket = ResourcePacket<GrvtPostProcess,	BaseAPI::FramebufferBuildData>;


	/**
	*/
	class ResourceBuildQueue {
	private:

		std::deque<MeshPacket>			MeshQueue;
		std::deque<TexturePacket>		TextureQueue;
		std::deque<ShaderPacket>		ShaderQueue;
		std::deque<FramebufferPacket>	FramebufferQueue;
		std::deque<DeletePacket>		DeleteQueue;

	public:

		ResourceBuildQueue();
		~ResourceBuildQueue();

		/**
		* [MIDDLEWARE]
		* Adds a mesh to be built by OpenGL. 
		*/
		void QueueMeshForBuild(GrvtMesh* Mesh, BaseAPI::MeshBuildData Data);


		/**
		* [MIDDLEWARE]
		* Adds a texture to be built by OpenGL.
		*/
		void QueueTextureForBuild(GrvtTexture* Texture, BaseAPI::TextureBuildData Data);


		/**
		* [MIDDLEWARE]
		* Adds a texture to be built by OpenGL.
		*/
		void QueueShaderForBuild(GrvtShader* Shader, BaseAPI::ShaderBuildData Data);

	
		/**
		* [MIDDLEWARE]
		* Adds a framebuffer to be built by OpenGL.
		*/
		void QueuePostProcessForBuild(GrvtPostProcess* Framebuffer, BaseAPI::FramebufferBuildData Data);


		/**
		* [MIDDLEWARE]
		* Removes a GraphicsObject from the GPU.
		*/
		void QueueHandleForDelete(ObjHandle& Handle, GfxObjectType Type);


		/**
		* [MIDDLEWARE]
		* Called every frame inside of GravityApp object.
		* 
		* Listens for new packets in the different resource queues and tells OpenGL to build them.
		*/
		void Listen();

	};


	/**
	* [MIDDLEWARE]
	* This sets the global ResourceBuildQueue variable to an instance of it.
	*/
	ResourceBuildQueue* InitialiseBuildQueue();


	/**
	* [MIDDLEWARE]
	* Simply returns the global ResourceBuildQueue variable.
	*/
	ResourceBuildQueue* GetBuildQueue();


	/**
	* [MIDDLEWARE]
	* Releases the ResourceBuildQueue.
	*
	* WARNING: Only call this function your program is shutting down. Make sure there are no more resource packets in it.
	*/
	void ReleaseBuildQueue();


	/**
	* [MIDDLEWARE]
	* Packages the mesh to and sends it to the ResourceBuildQueue.
	* If mesh set up requires more manual intervention, e.g: manually setting up the vertex attribute pointers, call AddMeshForBuild from ResourceBuildQueue.
	*
	* WARNING: This function allocates temporary memory on the heap. It is deleted once the Mesh has been successfully built.
	*/
	void PackageMeshForBuild(GrvtMesh* MeshSrc);


	/**
	* [MIDDLEWARE]
	* A mid level API to load textures from file into engine.
	*/
	void PackageTextureForBuild(GrvtTexture* TextureSrc);


	/**
	* [MIDDLEWARE]
	* A mid level API to build shaders from file into the engine.
	*/
	void PackageShaderForBuild(GrvtShader* ShaderSrc);


	/**
	* [MIDDLEWARE]
	* A mid level API to build framebuffers into the engine.
	*/
	void PackageFramebufferForBuild(GrvtPostProcess* FramebufferSrc);

	/**
	* [MIDDLEWARE]
	* A mid level API to retrieve shader program's uniform types.
	*/
	void GetUniformType(uint32 Type, AttrType& Main, AttrSubType& Sub);

}