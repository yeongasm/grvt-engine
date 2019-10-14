#pragma once


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
	size_t	PtrOffset;

	VertexAttribPointer();
	VertexAttribPointer(uint32 Index, int32 Size, int32 Stride, size_t Offset);

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
* RenderbufferBuildData data structure.
*/
struct RenderbufferBuildData {

	int32	Width;
	int32	Height;
	uint32	InternalFormat;

	RenderbufferBuildData();

	RenderbufferBuildData(const RenderbufferBuildData &Rhs);
	RenderbufferBuildData& operator= (const RenderbufferBuildData &Rhs);

	RenderbufferBuildData(RenderbufferBuildData &&Rhs);
	RenderbufferBuildData& operator= (RenderbufferBuildData &&Rhs);

	~RenderbufferBuildData();

};


/**
* [BASEAPI]
* FramebufferAttachment data structure.
*
* Specify the attachment that is to be attached onto a Framebuffer.
*/
struct FramebufferAttachment {

	ObjHandle		*Handle;
	uint32			Attachment;
	bool			Draw;

	FramebufferAttachment();
	FramebufferAttachment(ObjHandle *SrcHandle, uint32 SourceAttachment, bool DrawBuffer);

	FramebufferAttachment(const FramebufferAttachment &Rhs);
	FramebufferAttachment& operator= (const FramebufferAttachment &Rhs);

	FramebufferAttachment(FramebufferAttachment &&Rhs);
	FramebufferAttachment& operator= (FramebufferAttachment &&Rhs);

	~FramebufferAttachment();

};


/**
* [BASEAPI]
* FramebufferBuildData data structure.
*
*/
struct FramebufferBuildData {
private:

	using Attachments = Array<FramebufferAttachment>;

public:

	Attachments Attachment;

	FramebufferBuildData();

	FramebufferBuildData(const FramebufferBuildData &Rhs);
	FramebufferBuildData& operator= (const FramebufferBuildData &Rhs);

	FramebufferBuildData(FramebufferBuildData &&Rhs);
	FramebufferBuildData& operator= (FramebufferBuildData &&Rhs);

	~FramebufferBuildData();
};


namespace BaseAPI {

	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a mesh.
	*/
	void BuildMesh(ObjHandle &VAO, ObjHandle &VBO, ObjHandle &EBO, MeshBuildData &Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a texture.
	*/
	void BuildTexture(ObjHandle &Handle, TextureBuildData &Data);


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
	* An OpenGL wrapper to create a frambuffer.
	*/
	void BuildFramebuffer(ObjHandle &Handle, FramebufferBuildData &Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a renderbuffer.
	*/
	void BuildRenderbuffer(ObjHandle &Handle, RenderbufferBuildData &Data);
}
