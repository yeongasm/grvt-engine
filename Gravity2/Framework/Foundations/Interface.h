#pragma once


/**
* Forward declare all higher level graphic objects signature.
*/
class GrvtMesh;
class GrvtModel;
class GrvtTexture;
class GrvtShader;
class GrvtPostProcess;


/**
* Mid Level API a.k.a Middleware.
*
* The data structure and functions in this file bridges the gap between the high-level API which are foward declared above with the low-level API.
* It designed to be generic meaning only data structures that are commonly used in other objects should be passed into the low-level API.
*/


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

	MeshPacket(const MeshPacket &Rhs);
	MeshPacket& operator= (const MeshPacket &Rhs);

	MeshPacket(MeshPacket &&Rhs);
	MeshPacket& operator= (MeshPacket &&Rhs);

};


/**
* [MIDDLEWARE]
* TexturePacket data structure.
*
* A TexturePacket is required to bridge the gap between the high-level abstraction API with the low-level API.
* Low-level API can only process texture data that is contained inside of this data structure.
*/
struct TexturePacket {

	TextureObj			*TexturePtr;
	TextureBuildData	BuildData;

	TexturePacket();
	~TexturePacket();
	
	TexturePacket(TextureObj *Resource, TextureBuildData Data);

	TexturePacket(const TexturePacket &Rhs);
	TexturePacket& operator= (const TexturePacket &Rhs);

	TexturePacket(TexturePacket &&Rhs);
	TexturePacket& operator= (TexturePacket &&Rhs);

};


/**
* [MIDDLEWARE]
* ShaderPacket data structure.
*
* A ShaderPacket is required to bridge the gap between the high-level abstraction API with the low-level API.
* Low-level API can only process string data that is contained indside of this data structure.
*/
struct ShaderPacket {

	ShaderObj		*ShaderPtr;
	ShaderBuildData BuildData;

	ShaderPacket();
	~ShaderPacket();

	ShaderPacket(ShaderObj *Resource, ShaderBuildData Data);

	ShaderPacket(const ShaderPacket &Rhs);
	ShaderPacket& operator= (const ShaderPacket &Rhs);

	ShaderPacket(ShaderPacket &&Rhs);
	ShaderPacket& operator= (ShaderPacket &&Rhs);

};


/**
* [MIDDLEWARE]
* FramebufferPacket data structure.
*
* A FramebufferPacket is required to bridge the gap between the high-level abstraction API with the low level API.
* Low-level API can only process framebuffer data that is contained inside of this data structure.
*/
struct FramebufferPacket {

	PostProcessObj			*PostProcessPtr;
	FramebufferBuildData	BuildData;

	FramebufferPacket();
	~FramebufferPacket();

	FramebufferPacket(PostProcessObj *Resource, FramebufferBuildData Data);

	FramebufferPacket(const FramebufferPacket &Rhs);
	FramebufferPacket& operator= (const FramebufferPacket &Rhs);

	FramebufferPacket(FramebufferPacket &&Rhs);
	FramebufferPacket& operator= (FramebufferPacket &&Rhs);

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
* ResourceBuildQueue data structure.
*
* A global instance is already set up upon the application's creation.
*
* [WARNING] DO NOT CREATE AN INSTANCE OF THIS CLASS! THERE SHOULD ONLY BE ONE!
* TODO(Afiq):
* Implement for shaders
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
	void AddMeshForBuild			(MeshObj *Mesh, MeshBuildData Data);


	/**
	* [MIDDLEWARE]
	* Adds a texture to be built by OpenGL.
	*/
	void AddTextureForBuild			(TextureObj *Texture, TextureBuildData Data);


	/**
	* [MIDDLEWARE]
	* Adds a texture to be built by OpenGL.
	*/
	void AddShaderForBuild			(ShaderObj *Shader, ShaderBuildData Data);

	
	/**
	* [MIDDLEWARE]
	* Adds a framebuffer to be built by OpenGL.
	*/
	void AddPostprocessForBuild		(PostProcessObj *Framebuffer, FramebufferBuildData Data);


	/**
	* [MIDDLEWARE]
	* Removes a GraphicsObject from the GPU.
	*/
	void AddHandleForDelete			(ObjHandle &Handle, GfxObjectType Type);


	/**
	* [MIDDLEWARE]
	* Called every frame inside of GravityApp object.
	* 
	* Listens for new packets in the different resource queues and tells OpenGL to build them.
	*/
	void Listen();

};


namespace Middleware {


	/**
	* [MIDDLEWARE]
	* Simply returns the global ResourceBuildQueue variable.
	*/
	ResourceBuildQueue* GetBuildQueue		();


	/**
	* [MIDDLEWARE]
	* This sets the global ResourceBuildQueue variable to an instance of it.
	*/
	ResourceBuildQueue*	InitialiseBuildQueue();

	/**
	* [MIDDLEWARE]
	* A mid level API for processing data loading by Assimp and being passed into the engine.
	*/
	void	ParseMeshFromAssimp			(const String Path, bool FlipUV, ModelObj *Model);

	/**
	* [MIDDLEWARE]
	* Packages the mesh to and sends it to the ResourceBuildQueue.
	* If mesh set up requires more manual intervention, e.g: manually setting up the vertex attribute pointers, call AddMeshForBuild from ResourceBuildQueue.
	*
	* WARNING: This function allocates temporary memory on the heap. It is deleted once the Mesh has been successfully built.
	*/
	void	PackageMeshForBuild			(MeshObj *MeshSrc);


	/**
	* [MIDDLEWARE]
	* A mid level API to load textures from file into engine.
	*/
	void	ParseTextureFromFile		(const String Path, TextureObj *Texture);


	/**
	* [MIDDLEWARE]
	* A mid level API to build shaders from file into the engine.
	*/		


	/**
	* [MIDDLEWARE]
	* A mid level API to build framebuffers into the engine.
	*/
	void	PackageFramebufferForBuild	(PostProcessObj *FramebufferSrc);


}