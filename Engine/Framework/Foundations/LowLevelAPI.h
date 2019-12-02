#pragma once


namespace BaseAPI 
{


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
	struct ENGINE_API VertexAttribPointer 
	{
		uint32	Index;
		int32	Size;
		int32	Stride;
		size_t	PtrOffset;

		VertexAttribPointer();
		~VertexAttribPointer();
	
		VertexAttribPointer(uint32 Index, int32 Size, int32 Stride, size_t Offset);

		VertexAttribPointer(const VertexAttribPointer& Rhs);
		VertexAttribPointer& operator= (const VertexAttribPointer& Rhs);

		VertexAttribPointer(VertexAttribPointer&& Rhs);
		VertexAttribPointer& operator= (VertexAttribPointer&& Rhs);
	};


	struct ENGINE_API MeshBuildData 
	{
		float				*Data;
		uint32				*Indices;
		size_t				Size;
		size_t				Length;

		Gfl::Array<VertexAttribPointer>	VertexAttribPointers;

		MeshBuildData();
		~MeshBuildData();

		MeshBuildData(const MeshBuildData& Rhs);
		MeshBuildData& operator= (const MeshBuildData& Rhs);

		MeshBuildData(MeshBuildData&& Rhs);
		MeshBuildData& operator= (MeshBuildData&& Rhs);
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
	* @param [REQUIRED] (uint32)		Default = 0;	Target	- Texture's target. E.g: GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP.
	* @param [REQUIRED] (uint32)		Default = 0;	Type	- Data type of the pixel. E.g: GL_UNSIGNED_BYTE, GL_UNSIGNED_INT.
	* @param [OPTIONAL] (uint32)		Default = 0;	Format	- Format of the pixel data. If unspecified, Gravity would generate one base on the image's pixel data. E.g: GL_RED, GL_RGB.
	* @param [REQUIRED] (Params)						parameters	- Texture's parameters. Specify in pairs using Pair() data structure or using initializer lists.
	*/
	struct ENGINE_API TextureBuildData 
	{
		void			*DataPtr;
		bool			Mipmap;
		bool			Cubemap;
		bool			Flip;
		int32			Width;
		int32			Height;
		uint32			Target;
		uint32			Type;
		uint32			Format;
		uint32			InternalFormat;

		Array<Pair<uint32, uint32>>	Parameters;

		TextureBuildData();
		~TextureBuildData();

		TextureBuildData(const TextureBuildData& Rhs);
		TextureBuildData& operator= (const TextureBuildData& Rhs);

		TextureBuildData(TextureBuildData&& Rhs);
		TextureBuildData& operator= (TextureBuildData&& Rhs);
	};


	/**
	* [BASEAPI]
	* Type of Shader.
	*/
	enum FoundationsShaderType : uint32 
	{
		GrvtFoundations_ShaderType_None		= 0xFF,	/** On first init only */
		GrvtFoundations_ShaderType_Vertex	= 0x00,
		GrvtFoundations_ShaderType_Fragment = 0x01,
		GrvtFoundations_ShaderType_Geometry = 0x02
	};


	/**
	* [BASEAPI]
	* Specify Shader's build information.
	*/
	struct ENGINE_API ShaderInfo 
	{
		char* SourceCode;
		FoundationsShaderType Type;

		ShaderInfo();
		~ShaderInfo();
	};


	/**
	* [BASEAPI]
	* ShaderBuildData data structure.
	*
	*/
	struct ENGINE_API ShaderBuildData 
	{
		Array<ShaderInfo> BuildInfo;

		ShaderBuildData();
		~ShaderBuildData();

		ShaderBuildData(const ShaderBuildData& Rhs);
		ShaderBuildData& operator= (const ShaderBuildData& Rhs);

		ShaderBuildData(ShaderBuildData&& Rhs);
		ShaderBuildData& operator= (ShaderBuildData&& Rhs);
	};


	/**
	* [BASEAPI]
	* FramebufferAttachment data structure.
	* Specify the attachment that is to be attached onto a Framebuffer.
	*
	* @param (uint32)	HandleId	- A copy to the Framebuffer's ObjectHandle Id.
	* @param (uint32)	Attachment	- The attachment's format (GL_COLOR_ATTACHMENTn | GL_DEPTH_ATTACHMENT | GL_DEPTH_STENCIL_ATTACHMENT).
	* @param (bool)		Draw		- Specify if the attachment would be used for drawing. Only works for colour attachments.
	*/
	struct ENGINE_API FramebufferAttachment 
	{
		ObjHandle*		Handle;
		uint32			Type;
		uint32			Count;

		FramebufferAttachment();
		~FramebufferAttachment();

		FramebufferAttachment(ObjHandle* SrcHandle, uint32 SourceAttachment, uint32 Index);

		FramebufferAttachment(const FramebufferAttachment& Rhs);
		FramebufferAttachment& operator= (const FramebufferAttachment& Rhs);

		FramebufferAttachment(FramebufferAttachment&& Rhs);
		FramebufferAttachment& operator= (FramebufferAttachment&& Rhs);
	};


	/**
	* [BASEAPI]
	* FramebufferBuildData data structure.
	*
	* @param (Array<FramebufferAttachment>) Attachment - A list of attachment for the framebuffer.
	*/
	struct ENGINE_API FramebufferBuildData 
	{
		Array<FramebufferAttachment> Attachments;
		int32 Width;
		int32 Height;

		FramebufferBuildData();
		~FramebufferBuildData();

		FramebufferBuildData(const FramebufferBuildData& Rhs);
		FramebufferBuildData& operator= (const FramebufferBuildData& Rhs);

		FramebufferBuildData(FramebufferBuildData&& Rhs);
		FramebufferBuildData& operator= (FramebufferBuildData&& Rhs);
	};


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a mesh.
	*/
	extern "C" ENGINE_API void BuildMesh			(ObjHandle& VAO, ObjHandle& VBO, ObjHandle& EBO, MeshBuildData& Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a texture.
	*/
	extern "C" ENGINE_API void BuildTexture		(ObjHandle& Handle, TextureBuildData& Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to compile a shader.
	*/
	extern "C" ENGINE_API bool CompileShader		(ObjHandle& Handle, uint32 Type, const char* SourceCode);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a shader program.
	*/
	extern "C" ENGINE_API void BuildShaderProgram	(ObjHandle& Handle, ShaderBuildData& Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to retrieve shader 
	*/


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
	extern "C" ENGINE_API void GenerateGenericTextureData(TextureBuildData& Data);


	/**
	* [BASEAPI]
	* An OpenGL wrapper to create a frambuffer.
	*/
	extern "C" ENGINE_API void BuildFramebuffer	(ObjHandle& Handle, FramebufferBuildData& Data);


}