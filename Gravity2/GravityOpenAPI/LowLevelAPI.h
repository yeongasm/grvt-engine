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
* [BASEAPI]
* GraphicsObject data structure.
*
* Low level API functionality.
* Never to be used for identifying graphic related objects as this is an abstract class.
*/
struct GraphicsObject {

	uint32	Id;
	uint32	Target;

	GraphicsObject();
	virtual ~GraphicsObject();

	GraphicsObject(const GraphicsObject &Rhs)				= delete;
	GraphicsObject& operator= (const GraphicsObject &Rhs)	= delete;

	GraphicsObject(GraphicsObject &&Rhs)					= delete;
	GraphicsObject& operator= (GraphicsObject &&Rhs)		= delete;

	operator uint32();

	/**
	* [BASEAPI]
	* Creates a derived object of the specified target's type.
	*
	* @param [REQUIRED] (uint32) Target - The buffer's target type.
	*/
	virtual bool Alloc(uint32 Target) = 0;

	/**
	* [BASEAPI]
	* Deletes this derived object from OpenGL and sets it's value to 0.
	*/
	virtual bool Delete() = 0;

	/**
	* [BASEAPI]
	* Binds the derived object and update it's state in OpenGL.
	*
	* Return false if no Target is specified (Only true for some objects).
	*/
	virtual bool Bind() = 0;

	/**
	* [BASEAPI]
	* Unbinds the derived object and reset OpenGL's state for the specified target.
	*
	* Return false if no Target is specified (Only true for some objects).
	*/
	virtual bool UnBind() = 0;

	/**
	* [BASEAPI]
	* Resets the values of this derived object.
	* Note, this does not delete the derived object from OpenGL. Be sure to keep track of the Id.
	*/
	virtual void Reset();
};


/**
* [BASEAPI]
* BufferObject data structure.
*
* Low level API functionality.
* Can be used for Buffer Objects for Meshes.
*/
struct BufferObject : public GraphicsObject {

	BufferObject();
	~BufferObject();

	BufferObject(const BufferObject &Rhs)				= delete;
	BufferObject& operator= (const BufferObject &Rhs)	= delete;

	BufferObject(BufferObject &&Rhs);
	BufferObject& operator= (BufferObject &&Rhs);
	
	using GraphicsObject::operator uint32;

	/**
	* [BASEAPI]
	* Creates a new buffer object of the specified target's type.
	*
	* @param [REQUIRED] (BufferTarget) Target - The buffer's target type.
	*/
	bool Alloc(uint32 Target);

	/**
	* [BASEAPI]
	* Deletes this buffer object from OpenGL and sets it's value to 0 and it's target to EMPTY_BUFFER_OBJECT.
	*/
	bool Delete();

	/**
	* [BASEAPI]
	* Binds the Buffer Object and update it's state in OpenGL.
	*
	* Return false if no Target is specified (EMPTY_BUFFER_OBJECT).
	*/
	bool Bind();

	/**
	* [BASEAPI]
	* Unbinds the Buffer object and reset OpenGL's state for the specified target.
	*
	* Return false if no Target is specified (EMPTY_BUFFER_OBJECT).
	*/
	bool UnBind();

	/**
	* [BASEAPI]
	* Resets the values of this buffer object.
	* Note, this does not delete the buffer object from OpenGL. Be sure to keep track of the Id.
	*/
	//void Reset();
};




/**
* [BASEAPI]
* VtxArrObject data structure.
*
* Low level API functionality.
* Vertex Array Object for OpenGL.
*
* NOTE(Afiq):
* Would be nice to have the VAO track which Buffer is attached to it. A future feature perhaps.
*/
struct VertArrayObj : public GraphicsObject {

	VertArrayObj();
	~VertArrayObj();

	VertArrayObj(const VertArrayObj &Rhs)				= delete;
	VertArrayObj& operator= (const VertArrayObj &Rhs)	= delete;

	VertArrayObj(VertArrayObj &&Rhs);
	VertArrayObj& operator= (VertArrayObj &&Rhs);

	using GraphicsObject::operator uint32;

	/**
	* [BASEAPI]
	* Creates a new Vertex Array Object (VAO).
	*
	* Do not provide anything into Target when creating a Vertex Array Object.
	*/
	bool Alloc(uint32 Target = 0);

	/**
	* [BASEAPI]
	* Deletes this Vertex Array Object (VAO) and remove it from OpenGL.
	*/
	bool Delete();

	/**
	* [BASEAPI]
	* Bind's this Vertex Array Object (VAO) and update OpenGL's state.
	*/
	bool Bind();

	/**
	* [BASEAPI]
	* Unbinds the Vertex Array Object from OpenGL.
	*/
	bool UnBind();

	/**
	* [BASEAPI]
	* Resets the value of this vertex array object.
	* Note, this does not delete the vertex array object from OpenGL. Be sure to keep track of the Id.
	*/
	//void Reset();
};


/**
* [BASEAPI]
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
* [BASEAPI]
* TextureID data structure.
*
* Low level API functionality.
* Texture ID for OpenGL.
*/
struct TextureID : public GraphicsObject {

	TextureID();
	~TextureID();

	TextureID(const TextureID &Rhs)				= delete;
	TextureID& operator= (const TextureID &Rhs) = delete;

	TextureID(TextureID &&Rhs);
	TextureID& operator= (TextureID &&Rhs);

	using GraphicsObject::operator uint32;

	/**
	* [BASEAPI]
	* Creates a new Texture.
	*/
	bool	Alloc(uint32 Target);

	/**
	* [BASEAPI]
	* Deletes this Texture and removes it from OpenGL.
	*/
	bool	Delete();

	/**
	* [BASEAPI]
	* Binds this Texture and updates OpenGL's state.
	*/
	bool	Bind();

	/**
	* [BASEAPI]
	* Unbinds the Texture from OpenGL.
	*/
	bool	UnBind();

	/**
	* [BASEAPI]
	* Resets the value of this Texture.
	* Note, this does not delete the Texture from OpenGL. Be sure to keep track of the Id.
	*/
	//void	Reset();
};


/**
* [BASEAPI]
* TextureBuildData data structure.
* NOTE: It is important to know that the type of DataPtr will be dependent on it's Type (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT & GL_UNSIGNED_INT).
*
* IMPORTANT: When copying from another TextureBuildData object, always ensure that DataPtr is freed IF it is allocated dynamically.
* TextureBuildData does not perform a deep copy as it was never intended to be assigned from another instance of this object.
*
* @param [REQUIRED] (void*)							DataPtr - Texture's data. The type of pointer assigned needs to be the same as the type specified in TextureCreationInfo's type parameter.
* @param [REQUIRED] (bool)			Default = 1;	Mipmap	- Generate mipmaps for OpenGL texture.
* @param [OPTIONAL] (bool)			Default = 0;	Cubemap - Setting this to true would make the texture into a cubemap texture.
* @param [OPTIONAL] (bool)			Default = 1;	Flip	- Flips the image and makes the 0.0 coordinate on the Y-axis be on the bottom side.
* @param [OPTIONAL] (int)			Default = 0;	Width	- Texture's width. Use only when creating an empty texture.
* @param [OPTIONAL] (int)			Default = 0;	Height	- Texture's height. Use only when creating an empty texture.
* @param [REQUIRED] (uint)			Default = 0;	Target	- Texture's target. E.g: GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP.
* @param [REQUIRED] (uint)			Default = 0;	Type	- Data type of the pixel. E.g: GL_UNSIGNED_BYTE, GL_UNSIGNED_INT.
* @param [OPTIONAL] (uint)			Default = 0;	Format	- Format of the pixel data. If unspecified, Gravity would generate one base on the image's pixel data. E.g: GL_RED, GL_RGB.
* @param [REQUIRED] (Params)						parameters	- Texture's parameters. Specify in pairs using Pair() data structure or using initializer lists.
*/
struct TextureBuildData {
private:

	using Params = Pair<uint32, uint32>;

public:

	void			*DataPtr;
	bool			Mipmap;
	bool			Cubemap;
	bool			Flip;
	int32			Width;
	int32			Height;
	uint32			Target;
	uint32			Type;
	uint32			Format;
	Array<Params>	Parameters;

	TextureBuildData();

	TextureBuildData(const TextureBuildData &Rhs);
	TextureBuildData(TextureBuildData &&Rhs);

	TextureBuildData& operator= (const TextureBuildData &Rhs);
	TextureBuildData& operator= (TextureBuildData &&Rhs);

	~TextureBuildData();
};


/**
* [BASEAPI]
* BufferID data structure.
*
* Low level API functionality.
* Framebuffer / Renderbuffer ID for OpenGL.
*/
struct BufferID : public GraphicsObject {

	uint32 InternalFormat;
	uint32 Attachment;

	BufferID();
	~BufferID();

	BufferID(const BufferID &Rhs) = delete;
	BufferID& operator= (const BufferID &Rhs) = delete;

	BufferID(BufferID &&Rhs);
	BufferID& operator= (BufferID &&Rhs);

	using GraphicsObject::operator uint32;

	/**
	* [BASEAPI]
	* Creates a new Framebuffer / Renderbuffer.
	*/
	bool	Alloc(uint32 Target);

	/**
	* [BASEAPI]
	* Deletes this Framebuffer / Renderbuffer and removes it from OpenGL.
	*/
	bool	Delete();

	/**
	* [BASEAPI]
	* Binds this Framebuffer / Renderbuffer and updates OpenGL's state.
	*/
	bool	Bind();

	/**
	* [BASEAPI]
	* Unbinds the Framebuffer / Renderbuffer from OpenGL.
	*/
	bool	UnBind();

	/**
	* [BASEAPI]
	* Resets the value of this Framebuffer / Renderbuffer.
	* Note, this does not delete the Framebuffer / Renderbuffer from OpenGL. Be sure to keep track of the Id.
	*/
	void	Reset();
};


/**
* TODO(Afiq):
* [BASEAPI]
* ScreenbufferBuildData data structure.
*
* @param [REQUIRED] (void*)							DataPtr - Texture's data. The type of pointer assigned needs to be the same as the type specified in TextureCreationInfo's type parameter.
*/
struct BufferBuildData {
	/**
	* Only information required for framebuffers.
	* Also required for renderbuffers.
	*/
	uint32	Target;

	/**
	* Information required for renderbuffers.
	*/
	uint32	InternalFormat;
	uint32	Attachment;
	int32	Width;
	int32	Height;

	BufferBuildData();

	BufferBuildData(const BufferBuildData &Rhs);
	BufferBuildData(BufferBuildData &&Rhs);

	BufferBuildData& operator= (const BufferBuildData &Rhs);
	BufferBuildData& operator= (BufferBuildData &&Rhs);

	~BufferBuildData();
};


namespace BaseAPI {

	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a mesh.
	*/
	void BuildMesh(VertArrayObj &VAO, BufferObject &VBO, BufferObject &EBO, MeshBuildData &Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to delete a mesh.
	*/
	void DeleteMesh(VertArrayObj &VAO, BufferObject &VBO, BufferObject &EBO);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a texture.
	*/
	void BuildTexture(TextureID &ID, TextureBuildData &Data);

	/**
	* [BASEAPI]
	* An OpenGL wrapper to set generic texture building data.
	*
	* TextureBuildData.Target		= GL_TEXTURE_2D;
	* TextureBuildData.Type			= GL_UNSIGNED_BYTE;
	* TextureBuildData.Format		= GL_RGB;
	* TextureBuildData.Parameters[0] = {GL_TEXTURE_WRAP_S, GL_REPEAT}
	* TextureBuildData.Parameters[1] = {GL_TEXTURE_WRAP_T, GL_REPEAT}
	* TextureBuildData.Parameters[2] = {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR}
	* TextureBuildData.Parameters[3] = {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
	*/
	void GenerateGenericTextureData(TextureBuildData &Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to delete a texture.
	*/
	void DeleteTexture(TextureID &ID);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a cubemap.
	*/

	/**
	* [BASEAPI]
	* An OpenGL wrapper to delete a cubemap.
	*/

	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a frambuffer.
	*/
	void BuildFramebuffer(BufferID &ID, BufferBuildData &Data);
}
