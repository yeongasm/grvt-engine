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
	* MeshPacket data structure.
	*
	* A MeshPacket is required to bridge the gap between the high-level abstraction API with the low-level API.
	* Low-level API can only process mesh data that is contained inside of this data structure.
	*/
	struct MeshPacket {

		GrvtMesh*		MeshPtr;
		MeshBuildData	BuildData;

		MeshPacket();
		~MeshPacket();

		MeshPacket(GrvtMesh* ResourcePtr, MeshBuildData BuildData);

	};


	/**
	* [MIDDLEWARE]
	* TexturePacket data structure.
	*
	* A TexturePacket is required to bridge the gap between the high-level abstraction API with the low-level API.
	* Low-level API can only process texture data that is contained inside of this data structure.
	*/
	struct TexturePacket {

		GrvtTexture*		TexturePtr;
		TextureBuildData	BuildData;

		TexturePacket();
		~TexturePacket();
	
		TexturePacket(GrvtTexture* Resource, TextureBuildData Data);

	};


	/**
	* [MIDDLEWARE]
	* ShaderPacket data structure.
	*
	* A ShaderPacket is required to bridge the gap between the high-level abstraction API with the low-level API.
	* Low-level API can only process string data that is contained indside of this data structure.
	*/
	struct ShaderPacket {

		GrvtShader*		ShaderPtr;
		ShaderBuildData BuildData;

		ShaderPacket();
		~ShaderPacket();

		ShaderPacket(GrvtShader* Resource, ShaderBuildData Data);

	};


	/**
	* [MIDDLEWARE]
	* FramebufferPacket data structure.
	*
	* A FramebufferPacket is required to bridge the gap between the high-level abstraction API with the low level API.
	* Low-level API can only process framebuffer data that is contained inside of this data structure.
	*/
	struct FramebufferPacket {

		GrvtPostProcess*		PostProcessPtr;
		FramebufferBuildData	BuildData;

		FramebufferPacket();
		~FramebufferPacket();

		FramebufferPacket(GrvtPostProcess* Resource, FramebufferBuildData Data);

	};


	/**
	* [MIDDLEWARE]
	* GfxObjectType enums.
	*
	* A list of available graphics object in the engine.
	* Used for identifying the derived class of GraphicsObject.
	*/
	enum GfxObjectType : uint32 {

		GFX_TYPE_NONE			= 0x00,	// Used only for initialisation.
		GFX_TYPE_MESHBUFFER		= 0x01,
		GFX_TYPE_VERTEXARRAY	= 0x02,
		GFX_TYPE_TEXTUREID		= 0x03,
		GFX_TYPE_SHADERID		= 0x04,
		GFX_TYPE_POSTPROCESS	= 0x05,
		GFX_TYPE_RENDERBUFFER	= 0x06

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
		void QueueMeshForBuild(GrvtMesh* Mesh, MeshBuildData Data);


		/**
		* [MIDDLEWARE]
		* Adds a texture to be built by OpenGL.
		*/
		void QueueTextureForBuild(GrvtTexture* Texture, TextureBuildData Data);


		/**
		* [MIDDLEWARE]
		* Adds a texture to be built by OpenGL.
		*/
		void QueueShaderForBuild(GrvtShader* Shader, ShaderBuildData Data);

	
		/**
		* [MIDDLEWARE]
		* Adds a framebuffer to be built by OpenGL.
		*/
		void QueuePostProcessForBuild(GrvtPostProcess* Framebuffer, FramebufferBuildData Data);


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
	* Simply returns the global ResourceBuildQueue variable.
	*/
	ResourceBuildQueue* GetBuildQueue();


	/**
	* [MIDDLEWARE]
	* This sets the global ResourceBuildQueue variable to an instance of it.
	*/
	ResourceBuildQueue*	InitialiseBuildQueue();

	/**
	* [MIDDLEWARE]
	* A mid level API for processing data loading by Assimp and being passed into the engine.
	*/
	//void	ParseMeshFromAssimp(const String Path, bool FlipUV, ModelObj *Model);

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
	void ParseTextureFromFile(const String Path, GrvtTexture* Texture);


	/**
	* [MIDDLEWARE]
	* A mid level API to build shaders from file into the engine.
	*/
	void Par


	/**
	* [MIDDLEWARE]
	* A mid level API to build framebuffers into the engine.
	*/
	void PackageFramebufferForBuild(GrvtPostProcess* FramebufferSrc);


}