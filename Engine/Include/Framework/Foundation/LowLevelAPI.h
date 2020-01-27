#pragma once

#ifndef GRAVITY_GL_LOWLEVELAPI
#define GRAVITY_GL_LOWLEVELAPI

#include "Minimal.h"
#include "Foundations.h"

namespace Grvt
{

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
		struct VertexAttribPointer
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


		struct MeshBuildData
		{
			float*	Data;
			uint32* Indices;
			size_t	Size;
			size_t	Length;

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
		* @param [OPTIONAL] (bool)			Default = 1;	Flip	- Flips the image and makes the 0.0 coordinate on the Y-axis be on the bottom side.
		* @param [OPTIONAL] (int)			Default = 0;	Width	- Texture's width. Use only when creating an empty texture.
		* @param [OPTIONAL] (int)			Default = 0;	Height	- Texture's height. Use only when creating an empty texture.
		* @param [REQUIRED] (uint32)		Default = 0;	Target	- Texture's target. E.g: GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP.
		* @param [REQUIRED] (uint32)		Default = 0;	Type	- Data type of the pixel. E.g: GL_UNSIGNED_BYTE, GL_UNSIGNED_INT.
		* @param [OPTIONAL] (uint32)		Default = 0;	Format	- Format of the pixel data. If unspecified, Gravity would generate one base on the image's pixel data. E.g: GL_RED, GL_RGB.
		* @param [REQUIRED] (Params)						parameters	- Texture's parameters. Specify in pairs using Pair() data structure or using initializer lists.
		*/
		struct TextureBuildData
		{
			Gfl::Array<Gfl::Pair<uint32, uint32>>	Parameters;
			float	BorderColour[4] = {0.0f};	/** Default: Black. Will not call glTexParameterfv by default. */

			uint32	Target;				/** Default: GL_TEXTURE_2D		*/
			uint32	Type;				/** Default: GL_UNSIGNED_BYTE	*/
			uint32	Format;				/** Default: Unspecified		*/
			uint32	InternalFormat;		/** Default: Unspecified		*/
			int32	Width;				/** Default: 0					*/
			int32	Height;				/** Default: 0					*/
			bool	Mipmap;				/** Default: True				*/
			bool	Flip;				/** Default: True				*/

			union
			{
				void* CubemapDataPtr[6]	= {nullptr};
				void* DataPtr;
			};


			ENGINE_API TextureBuildData();
			ENGINE_API ~TextureBuildData();

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
			GrvtFoundations_ShaderType_None = 0xFF,	/** On first init only */
			GrvtFoundations_ShaderType_Vertex = 0x00,
			GrvtFoundations_ShaderType_Fragment = 0x01,
			GrvtFoundations_ShaderType_Geometry = 0x02
		};


		/**
		* [BASEAPI]
		* Specify Shader's build information.
		*/
		struct ShaderInfo
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
		struct ShaderBuildData
		{
			Gfl::Array<ShaderInfo> BuildInfo;

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
		*/
		struct TextureAttachment
		{
			TextureBuildData BuildData;
			ObjHandle*		 Handle;
			uint32			 Type;

			TextureAttachment();
			~TextureAttachment();

			TextureAttachment(ObjHandle* SrcHandle, uint32 Type, TextureBuildData BuilData);

			TextureAttachment(const TextureAttachment& Rhs);
			TextureAttachment& operator= (const TextureAttachment& Rhs);

			TextureAttachment(TextureAttachment&& Rhs);
			TextureAttachment& operator= (TextureAttachment&& Rhs);
		};


		/**
		* [BASEAPI]
		* FramebufferBuildData data structure.
		*
		* @param (Array<FramebufferAttachment>) Attachment - A list of attachment for the framebuffer.
		*/
		struct FramebufferBuildData
		{
			Gfl::Array<TextureAttachment> Attachments;
			int32	Width;
			int32	Height;

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
		void BuildMesh(ObjHandle& VAO, ObjHandle& VBO, ObjHandle& EBO, MeshBuildData& Data);


		/**
		* [BASEAPI]
		* An OpenGL wrapper to create a texture.
		*/
		void BuildTexture(ObjHandle& Handle, TextureBuildData& Data);


		/**
		* [BASEAPI]
		* An OpenGL wrapper to compile a shader.
		*/
		bool CompileShader(ObjHandle& Handle, uint32 Type, const char* SourceCode);


		/**
		* [BASEAPI]
		* An OpenGL wrapper to create a shader program.
		*/
		void BuildShaderProgram(ObjHandle& Handle, ShaderBuildData& Data);


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
		void GenerateGenericTextureData(TextureBuildData& Data);


		/**
		* [BASEAPI]
		* An OpenGL wrapper to set generice cubemap building data.
		*
		* TextureBuildData.Target = GL_TEXTURE_CUBE_MAP;
		* TextureBuildData.Type = GL_UNSIGNED_BYTE;
		* TextureBuildData.Format = GL_RGB;
		* TextureBuildData.Parameters[0] = {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}
		* TextureBuildData.Parameters[1] = {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}
		* TextureBuildData.Parameters[2] = {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}
		* TextureBuildData.Parameters[3] = {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
		* TextureBuildData.Parameters[4] = {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
		*/
		void GenerateGenericCubemapData(TextureBuildData& Data);


		/**
		* [BASEAPI]
		* An OpenGL wrapper to create a frambuffer.
		*/
		void BuildFramebuffer(ObjHandle& Handle, FramebufferBuildData& Data);

	}

}

#endif // !GRAVITY_GL_LOWLEVELAPI