#pragma once


/**
* TODO(Afiq):
* Forward declare all of the high level API data structures.
*/
class Mesh;
class Scene;
class Texture;
class Shader;


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


/**
* ResourceBuildQueue data structure.
*
* A global instance is already set up upon the application's creation.
*
* [WARNING] DO NOT CREATE AN INSTANCE OF THIS CLASS! THERE SHOULD ONLY BE ONE!
*/
class ResourceBuildQueue {
private:

	std::deque<MeshPacket> MeshQueue;

public:

	ResourceBuildQueue();
	~ResourceBuildQueue();

	/**
	* [MIDDLEWARE]
	* Adds a mesh to be built by OpenGL.
	* 
	*/
	void AddMeshForBuild(Mesh *Mesh, MeshBuildData Data);

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
	void				SetBuildQueue		(ResourceBuildQueue *BuildQueue);

	/**
	* [MIDDLEWARE]
	* Simply returns the global ResourceBuildQueue variable.
	*/
	ResourceBuildQueue* GetBuildQueue		();

	/**
	* [MIDDLEWARE]
	* A mid level API for processing data loading by Assimp and being passed into the engine.
	*/
	void				ParseMeshFromAssimp	(const String Path, bool FlipUV, Scene *Scene);

	/**
	* [MIDDLEWARE]
	* Packages the mesh to and sends it to the ResourceBuildQueue.
	*/
	void				PackageMeshForBuild	(Mesh *MeshSrc);
}