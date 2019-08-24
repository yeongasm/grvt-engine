#pragma once


/**
* NOTE(Afiq):
* OpenWrapper is a low level API to communicate with OpenGL.
* None of these data structures should hold data that represents the high level API.
*/


using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;

using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;


/**
* BufferObject data structure.
*
* Low level API functionality.
* Generic Buffer Object for OpenGL.
*/
struct BufferObject {

	uint32	Id;
	uint32	Target;

	BufferObject();
	~BufferObject();

	BufferObject(const BufferObject &Rhs)				= delete;
	BufferObject& operator= (const BufferObject &Rhs)	= delete;

	BufferObject(BufferObject &&Rhs);
	BufferObject& operator= (BufferObject &&Rhs);

	operator uint32();

	/**
	* Creates a new buffer object of the specified target's type.
	*
	* @param [REQUIRED] (BufferTarget) Target - The buffer's target type.
	*/
	bool Alloc(uint32 Target);

	/**
	* Deletes this buffer object from OpenGL and sets it's value to 0 and it's target to EMPTY_BUFFER_OBJECT.
	*/
	bool Delete();

	/**
	* Binds the Buffer Object and update it's state in OpenGL.
	*
	* Return false if no Target is specified (EMPTY_BUFFER_OBJECT).
	*/
	bool Bind();

	/**
	* Unbinds the Buffer object and reset OpenGL's state for the specified target.
	*
	* Return false if no Target is specified (EMPTY_BUFFER_OBJECT).
	*/
	bool UnBind();

	/**
	* Resets the values of this buffer object.
	* Note, this does not delete the buffer object from OpenGL. Be sure to keep track of the Id.
	*/
	void Reset();
};


/**
* VtxArrObject data structure.
*
* Low level API functionality.
* Vertex Array Object for OpenGL.
*
* NOTE(Afiq):
* Would be nice to have the VAO track which Buffer is attached to it. A future feature perhaps.
*/
struct VertArrayObj {

	uint32 Id;

	VertArrayObj();
	~VertArrayObj();

	VertArrayObj(const VertArrayObj &Rhs)				= delete;
	VertArrayObj& operator= (const VertArrayObj &Rhs)	= delete;

	VertArrayObj(VertArrayObj &&Rhs);
	VertArrayObj& operator= (VertArrayObj &&Rhs);

	operator uint32();

	/**
	* Creates a new Vertex Array Object (VAO).
	*/
	bool Alloc();

	/**
	* Deletes this Vertex Array Object (VAO) and remove it from OpenGL.
	*/
	bool Delete();

	/**
	* Bind's this Vertex Array Object (VAO) and update OpenGL's state.
	*/
	bool Bind();

	/**
	* Unbinds the Vertex Array Object from OpenGL.
	*/
	void UnBind();

	/**
	* Resets the value of this vertex array object.
	* Note, this does not delete the vertex array object from OpenGL. Be sure to keep track of the Id.
	*/
	void Reset();
};


/**
* Mesh Vertex Attribute Pointers.
*
* Freedom is given to assign the index to any data in the mesh.
* However, for consistency and the perk of using the same layout in various shaders, it is recommended to specify the following index for the following data:
*
* Index: 0 - Mesh vertex positions.
* Index: 1 - Mesh normals.
* Index: 2 - Mesh UV coordinates.
* Index: 3 - Mesh tangents.
* Index: 4 - Mesh bitangents.
*
* @param [REQUIRED] (uint32)	Index	- Data's layout index inside of the shader.
* @param [REQUIRED] (int32)		Size	- Represent the number of components per generic vertex attribute.
* @param [REQUIRED] (int32)		Stride	- Needs to be manually provided. Automating this would reduce flexibility to generate Meshes for different type of objects. Eg: Models & Terrains.
* @param [REQURIED] (uint32)	Pointer - Pointer offset to the data inside of the vertex buffer.
*/
struct VertexAttribPointer {

	uint32	Index;
	int32	Size;
	int32	Stride;
	size_t	Pointer;

	VertexAttribPointer();
	VertexAttribPointer(uint32 Index, int32 Size, int32 Stride, size_t Pointer);

	VertexAttribPointer(const VertexAttribPointer &Rhs);
	VertexAttribPointer& operator= (const VertexAttribPointer &Rhs);

	VertexAttribPointer(VertexAttribPointer &&Rhs);
	VertexAttribPointer& operator= (VertexAttribPointer &&Rhs);

	~VertexAttribPointer();
};


struct MeshBuildData {
private:
	using VertexAttribsArr	= Array<VertexAttribPointer>;

public:
	float				*Data;
	uint32				*Indices;
	size_t				Size;
	size_t				Length;
	VertexAttribsArr	VertexAttribPointers;

	MeshBuildData();

	MeshBuildData(const MeshBuildData &Rhs);
	MeshBuildData& operator= (const MeshBuildData &Rhs);

	MeshBuildData(MeshBuildData &&Rhs);
	MeshBuildData& operator= (MeshBuildData &&Rhs);

	~MeshBuildData();
};


/**
* TextureID data structure.
*
* Low level API functionality.
* Texture id for OpenGL.
*/
struct TextureID {

	uint32 Id;
	uint32 Target;

	TextureID();
	~TextureID();

	TextureID(const TextureID &Rhs)				= delete;
	TextureID& operator= (const TextureID &Rhs) = delete;

	TextureID(TextureID &&Rhs);
	TextureID& operator= (TextureID &&Rhs);

	operator uint32();

	/**
	* Creates a new Texture.
	*/
	bool	Alloc(uint32 Target);

	/**
	* Deletes this Texture and removes it from OpenGL.
	*/
	bool	Delete();

	/**
	* Binds this Texture and updates OpenGL's state.
	*/
	bool	Bind();

	/**
	* Unbinds the Texture from OpenGL.
	*/
	bool	UnBind();

	/**
	* Resets the value of this Texture.
	* Note, this does not delete the Texture from OpenGL. Be sure to keep track of the Id.
	*/
	void	Reset();
};


//struct TextureCreationInfo;


/**
* TextureBuildData data structure.
*
* @param [REQUIRED] (void*)	DataPtr - Texture's data. The type of pointer assigned needs to be the same as the type specified in TextureCreationInfo's type parameter.
* @param [REQUIRED] (TextureCreationInfo) TextureInfo - Texture's build information.
*/
//struct TextureBuildData {
//
//	void				*DataPtr;
//	TextureCreationInfo TextureInfo;
//
//	TextureBuildData();
//
//	TextureBuildData(const TextureBuildData &Rhs);
//	TextureBuildData(TextureBuildData &&Rhs);
//
//	TextureBuildData& operator= (const TextureBuildData &Rhs);
//	TextureBuildData& operator= (TextureBuildData &&Rhs);
//
//	~TextureBuildData();
//};


/**
* An OpenGL wrapper to create a mesh.
*/
void OpenWrapBuildMesh(VertArrayObj &VAO, BufferObject &VBO, BufferObject &EBO, MeshBuildData &Data);


/**
* An OpenGL wrapper to delete a mesh.
*/
void OpenWrapDeleteMesh(VertArrayObj &VAO, BufferObject &VBO, BufferObject &EBO);


/**
* An OpenGL wrapper to create a texture.
*/
//void OpenWrapBuildTexture(TextureID &Texture, void *Data, TextureCreationInfo &Info);


/**
* An OpenGL wrapper to delete a texture.
*/
//void OpenWrapDeleteTexture(TextureID &Texture);