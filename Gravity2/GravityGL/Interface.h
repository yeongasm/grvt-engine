#pragma once


/**
* TODO(Afiq):
* Forward declare all of the high level API data structures.
* 
* Future tasks:
* 1. Make a low level API for texture and shaders.
* 2. Generate the shader in engine and modularize it.
* 3. When framebuffers are implemented, do the same for it.
*/
class Mesh;
class Scene;
class Texture;
class Shader;
class RenderBuffer;
class PostProcess;


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
	Mesh			*MeshPtr;
	MeshBuildData	BuildData;

	MeshPacket();
	MeshPacket(Mesh *Resource, MeshBuildData Data);

	MeshPacket(const MeshPacket &Rhs);
	MeshPacket(MeshPacket &&Rhs);

	MeshPacket& operator= (const MeshPacket &Rhs);
	MeshPacket& operator= (MeshPacket &&Rhs);

	~MeshPacket();
};

struct TextureCreationInfo;

/**
* [MIDDLEWARE]
* TexturePacket data structure.
*
* A TexturePacket is required to bridge the gap between the high-level abstraction API with the low-level API.
* Low-level API can only process texture data that is contained inside of this data structure.
*/
struct TexturePacket {
	Texture				*TexturePtr;
	TextureBuildData	BuildData;

	TexturePacket();
	TexturePacket(Texture *Resource, TextureBuildData Data);

	TexturePacket(const TexturePacket &Rhs);
	TexturePacket(TexturePacket &&Rhs);

	TexturePacket& operator= (const TexturePacket &Rhs);
	TexturePacket& operator= (TexturePacket &&Rhs);

	~TexturePacket();
};


/**
* [MIDDLEWARE]
* RenderbufferPacket data structure.
*
* A RenderbufferPacket is required to bridge the gap between the high-level abstraction API with the low level API.
* Low-level API can only process framebuffer data that is contained inside of this data structure.
*/
struct RenderbufferPacket {

	RenderBuffer			*RenderBufferPtr;
	RenderbufferBuildData	BuildData;

	RenderbufferPacket();
	RenderbufferPacket(RenderBuffer *Resource, RenderbufferBuildData Data);

	RenderbufferPacket(const RenderbufferPacket &Rhs);
	RenderbufferPacket& operator= (const RenderbufferPacket &Rhs);

	RenderbufferPacket(RenderbufferPacket &&Rhs);
	RenderbufferPacket& operator= (RenderbufferPacket &&Rhs);

	~RenderbufferPacket();

};


/**
* [MIDDLEWARE]
* FramebufferPacket data structure.
*
* A FramebufferPacket is required to bridge the gap between the high-level abstraction API with the low level API.
* Low-level API can only process framebuffer data that is contained inside of this data structure.
*/
struct FramebufferPacket {

	PostProcess				*PostProcessPtr;
	FramebufferBuildData	BuildData;

	FramebufferPacket();
	FramebufferPacket(PostProcess *Resource, FramebufferBuildData Data);

	FramebufferPacket(const FramebufferPacket &Rhs);
	FramebufferPacket& operator= (const FramebufferPacket &Rhs);

	FramebufferPacket(FramebufferPacket &&Rhs);
	FramebufferPacket& operator= (FramebufferPacket &&Rhs);

	~FramebufferPacket();

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
* NOTE(Afiq):
* 
*/
class ResourceBuildQueue {
private:

	std::deque<MeshPacket>			MeshQueue;
	std::deque<TexturePacket>		TextureQueue;
	std::deque<FramebufferPacket>	FramebufferQueue;
	std::deque<RenderbufferPacket>	RenderbufferQueue;
	std::deque<DeletePacket>		DeleteQueue;

public:

	ResourceBuildQueue();
	~ResourceBuildQueue();

	/**
	* [MIDDLEWARE]
	* Adds a mesh to be built by OpenGL. 
	*/
	void AddMeshForBuild(Mesh *Mesh, MeshBuildData Data);


	/**
	* [MIDDLEWARE]
	* Adds a texture to be built by OpenGL.
	*/
	void AddTextureForBuild(Texture *Texture, TextureBuildData Data);

	
	/**
	* [MIDDLEWARE]
	* Adds a framebuffer to be built by OpenGL.
	*/
	void AddPostprocessForBuild(PostProcess *Framebuffer, FramebufferBuildData Data);


	/**
	* [MIDDLEWARE]
	* Adds a renderbuffer to be built by OpenGL.
	*/
	void AddRenderBufferForBuild(RenderBuffer *Renderbuffer, RenderbufferBuildData Data);


	/**
	* [MIDDLEWARE]
	* Removes a GraphicsObject from the GPU.
	*/
	void AddHandleForDelete(ObjHandle &Handle, GfxObjectType Type);


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
	* This sets the global ResourceBuildQueue variable to an instance of it.
	*/
	void				SetBuildQueue			(ResourceBuildQueue *BuildQueue);

	/**
	* [MIDDLEWARE]
	* Simply returns the global ResourceBuildQueue variable.
	*/
	ResourceBuildQueue* GetBuildQueue			();

	/**
	* [MIDDLEWARE]
	* A mid level API for processing data loading by Assimp and being passed into the engine.
	*/
	void				ParseMeshFromAssimp		(const String Path, bool FlipUV, Scene *Scene);

	/**
	* [MIDDLEWARE]
	* Packages the mesh to and sends it to the ResourceBuildQueue.
	* If mesh set up requires more manual intervention, e.g: manually setting up the vertex attribute pointers, call AddMeshForBuild from ResourceBuildQueue.
	*
	* WARNING: This function allocates temporary memory on the heap. It is deleted once the Mesh has been successfully built.
	*/
	void				PackageMeshForBuild		(Mesh *MeshSrc);


	/**
	* [MIDDLEWARE]
	* A mid level API to load textures from file into engine.
	*/
	void				ParseTextureFromFile	(const String Path, Texture *Texture);
}