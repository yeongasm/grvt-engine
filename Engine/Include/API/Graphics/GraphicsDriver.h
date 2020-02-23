#pragma once

#ifndef GRAVITY_API_GRAPHICS_DRIVER
#define GRAVITY_API_GRAPHICS_DRIVER

#include "Minimal.h"
#include "glad/glad.h"

namespace Grvt
{

	enum class HandleType : uint32
	{
		Handle_None			= 0xFF,
		Handle_Buffer		= 0x00,
		Handle_VertexArray	= 0x01,
		Handle_Texture		= 0x02,
		Handle_Shader		= 0x03,
		Handle_Framebuffer	= 0x04,
	};

	class GfxHandle
	{
	private:

		GfxHandle(const GfxHandle& Rhs)				= delete;
		GfxHandle& operator= (const GfxHandle& Rhs) = delete;

	public:

		uint32 Id		= 0;
		uint32 Target	= 0;

		GfxHandle()  = default;
		~GfxHandle() = default;

		GfxHandle(GfxHandle&& Rhs)				= default;
		GfxHandle& operator= (GfxHandle&& Rhs)	= default;
	};

	enum GfxVarType : uint32
	{
		Gfx_Type_None		= GL_NONE,
		Gfx_Type_Boolean	= GL_BOOL,
		Gfx_Type_UByte		= GL_UNSIGNED_BYTE,
		Gfx_Type_UInteger	= GL_UNSIGNED_INT,
		Gfx_Type_Integer	= GL_INT,
		Gfx_Type_HalfFloat	= GL_HALF_FLOAT,
		Gfx_Type_Float		= GL_FLOAT,
		Gfx_Type_Double		= GL_DOUBLE,
		Gfx_Type_Sampler1D	= GL_SAMPLER_1D,
		Gfx_Type_Sampler2D	= GL_SAMPLER_2D,
		Gfx_Type_Sampler3D	= GL_SAMPLER_3D,
		Gfx_Type_SamplerCube = GL_SAMPLER_CUBE,
		Gfx_Type_Vec2f		= GL_FLOAT_VEC2,
		Gfx_Type_Vec2d		= GL_DOUBLE_VEC2,
		Gfx_Type_Vec3f		= GL_FLOAT_VEC3,
		Gfx_Type_Vec3d		= GL_DOUBLE_VEC3,
		Gfx_Type_Vec4f		= GL_FLOAT_VEC4,
		Gfx_Type_Vec4d		= GL_DOUBLE_VEC4,
		Gfx_Type_Mat2f		= GL_FLOAT_MAT2,
		Gfx_Type_Mat2d		= GL_DOUBLE_MAT2,
		Gfx_Type_Mat3f		= GL_FLOAT_MAT3,
		Gfx_Type_Mat3d		= GL_DOUBLE_MAT3,
		Gfx_Type_Mat4f		= GL_FLOAT_MAT4,
		Gfx_Type_Mat4d		= GL_DOUBLE_MAT4
	};

	enum GfxTexType : uint32
	{
		Gfx_Texture_1D = GL_TEXTURE_1D,
		Gfx_Texture_2D = GL_TEXTURE_2D,
		Gfx_Texture_3D = GL_TEXTURE_3D,
		Gfx_Texture_CubeMap = GL_TEXTURE_CUBE_MAP
	};

	enum GfxTexParam : uint32
	{
		Gfx_TexParam_MinFilter = GL_TEXTURE_MIN_FILTER,
		Gfx_TexParam_MagFilter = GL_TEXTURE_MAG_FILTER,
		Gfx_TexParam_WrapS = GL_TEXTURE_WRAP_S,
		Gfx_TexParam_WrapT = GL_TEXTURE_WRAP_T,
		Gfx_TexParam_WrapR = GL_TEXTURE_WRAP_R
	};

	enum GfxCompareFuncs : uint32
	{
		Gfx_CmpFunc_Lequal	= GL_LEQUAL,
		Gfx_CmpFunc_Gequal	= GL_GEQUAL,
		Gfx_CmpFunc_Less	= GL_LESS,
		Gfx_CmpFunc_Greater = GL_GREATER,
		Gfx_CmpFunc_Equal	= GL_EQUAL,
		Gfx_CmpFunc_NotEqual = GL_NOTEQUAL,
		Gfx_CmpFunc_Always	= GL_ALWAYS,
		Gfx_CmpFunc_Never	= GL_NEVER
	};

	enum GfxWrapFunc : uint32
	{
		Gfx_Wrap_Repeat			= GL_REPEAT,
		Gfx_Wrap_MirroredRepeat = GL_MIRRORED_REPEAT,
		Gfx_Wrap_ClampToEdge	= GL_CLAMP_TO_EDGE,
		Gfx_Wrap_ClampToBorder	= GL_CLAMP_TO_BORDER,
		Gfx_Wrap_MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	};

	enum GfxMipFunc : uint32
	{
		Gfx_Mip_Nearest = GL_NEAREST,
		Gfx_Mip_Linear	= GL_LINEAR,
		Gfx_Mip_NearestMipMapNearest	= GL_NEAREST_MIPMAP_NEAREST,
		Gfx_Mip_NearestMipMapLinear		= GL_NEAREST_MIPMAP_LINEAR,
		Gfx_Mip_LinearMipMapNearest		= GL_LINEAR_MIPMAP_NEAREST,
		Gfx_Mip_LinearMipMapLinear		= GL_LINEAR_MIPMAP_LINEAR
	};

	enum GfxAttachment : uint32
	{
		Gfx_Attachment_DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
		Gfx_Attachment_Depth		= GL_DEPTH_ATTACHMENT,
		Gfx_Attachment_Colour0		= GL_COLOR_ATTACHMENT0,
		Gfx_Attachment_Colour1		= GL_COLOR_ATTACHMENT1,
		Gfx_Attachment_Colour2		= GL_COLOR_ATTACHMENT2,
		Gfx_Attachment_Colour3		= GL_COLOR_ATTACHMENT3,
		Gfx_Attachment_Colour4		= GL_COLOR_ATTACHMENT4,
		Gfx_Attachment_Colour5		= GL_COLOR_ATTACHMENT5,
		Gfx_Attachment_Colour6		= GL_COLOR_ATTACHMENT6,
		Gfx_Attachment_Colour7		= GL_COLOR_ATTACHMENT7,
		Gfx_Attachment_Colour8		= GL_COLOR_ATTACHMENT8,
		Gfx_Attachment_Colour9		= GL_COLOR_ATTACHMENT9,
		Gfx_Attachment_Colour10		= GL_COLOR_ATTACHMENT10,
		Gfx_Attachment_Colour11		= GL_COLOR_ATTACHMENT11,
		Gfx_Attachment_Colour12		= GL_COLOR_ATTACHMENT12,
		Gfx_Attachment_Colour13		= GL_COLOR_ATTACHMENT13,
		Gfx_Attachment_Colour14		= GL_COLOR_ATTACHMENT14,
		Gfx_Attachment_Colour15		= GL_COLOR_ATTACHMENT15,
		Gfx_Attachment_Colour16		= GL_COLOR_ATTACHMENT16,
		Gfx_Attachment_Colour17		= GL_COLOR_ATTACHMENT17,
		Gfx_Attachment_Colour18		= GL_COLOR_ATTACHMENT18,
		Gfx_Attachment_Colour19		= GL_COLOR_ATTACHMENT19,
		Gfx_Attachment_Colour20		= GL_COLOR_ATTACHMENT20,
		Gfx_Attachment_Colour21		= GL_COLOR_ATTACHMENT21,
		Gfx_Attachment_Colour22		= GL_COLOR_ATTACHMENT22,
		Gfx_Attachment_Colour23		= GL_COLOR_ATTACHMENT23,
		Gfx_Attachment_Colour24		= GL_COLOR_ATTACHMENT24,
	};

	enum GfxFormats : uint32
	{
		Gfx_Format_Red	= GL_RED,
		Gfx_Format_Rg	= GL_RG,
		Gfx_Format_Rgb	= GL_RGB,
		Gfx_Format_Rgba = GL_RGBA,
		Gfx_Format_Depth = GL_DEPTH_COMPONENT,
		Gfx_Format_DepthStencil = GL_DEPTH_STENCIL,
		Gfx_Format_Rgb16F	= GL_RGB16F,
		Gfx_Format_Rgba16F	= GL_RGBA16F
	};

	struct VertexAttr
	{
		Gfl::String	Name;
		GfxVarType	Type;
		uint32		Location;
		int32		Size;

		VertexAttr();
		~VertexAttr();
	};

	using UniformAttr = VertexAttr;

	namespace Driver
	{
		struct BufferSubData
		{
			int32	Offset = 0;
			size_t	Size = 0;
			void*	DataPtr = nullptr;
		};

		using BufSubDataArr = Gfl::Array<BufferSubData>;

		struct BufferBuildData
		{
			BufSubDataArr SubData;

			void*	DataPtr = nullptr;
			size_t	Size	= 0;
			uint32	Target	= 0;

			void AddData(uint32 Target, size_t Size, void* SrcData);
			void RemoveData();

			void PushSubData(size_t Size, uint32 Offset, void* SubData);
			void PopSubData();
		};

		struct MeshVertexAttribPointer
		{
			uint32	Index	= 0;
			int32	Size	= 0;
			int32	Stride	= 0;
			size_t	Offset	= 0;
			uint32	Divisor	= 0;
		};

		using MeshVertAttribs	= Gfl::Array<MeshVertexAttribPointer>;
		using BufferDataPair	= Gfl::Pair<GfxHandle*, BufferBuildData> ;
		using BufferDataArr		= Gfl::Array<BufferDataPair>;
		
		struct MeshBuildData
		{
			MeshVertAttribs Attributes;
			BufferDataArr	Buffers;

			size_t		Size	= 0;
			size_t		Length	= 0;

			MeshBuildData()  = default;
			~MeshBuildData() = default;

			void PushAttribute(uint32 Index, int32 Size, int32 Stride, size_t Offset, uint32 Divisor = 0);
			void PopAttribute();

			void PushBuffer(GfxHandle* Handle, const BufferBuildData& Data);
			void PopBuffer();
		};

		using TextureParameter = Gfl::Pair<uint32, uint32>;
		using TexParams = Gfl::Array<TextureParameter>;

		struct TextureBuildData
		{
			union
			{
				void* CubeMapDataPtr[6] = {nullptr};
				void* DataPtr;
			};

			TexParams	Parameters;
			float32		BorderColour[4] = {0.0f};
			
			float32 FilterLevel = 4.0f;
			uint32	Target	= GL_TEXTURE_2D;
			uint32	Type	= GL_UNSIGNED_BYTE;
			uint32	Format	= GL_RGBA;
			uint32	InternalFormat	= GL_RGBA;
			int32	Width	= 0;
			int32	Height	= 0;

			bool Mipmap	= true;
			bool Flip	= true;

			TextureBuildData()  = default;
			~TextureBuildData() = default;

			void PushTextureParameter(uint32 Parameter, uint32 Value);
			void PopTextureParameter();
		};

		struct ShaderBuildData
		{
			Gfl::String VertexShader;
			Gfl::String GeometryShader;
			Gfl::String FragmentShader;

			ShaderBuildData()  = default;
			~ShaderBuildData() = default;
		};

		struct FrameTexAttachment
		{
			TextureBuildData BuildData;
			GfxHandle*	Handle	= nullptr;
			uint32		Type	= 0;

			FrameTexAttachment()  = default;
			~FrameTexAttachment() = default;

			FrameTexAttachment(GfxHandle* AttachmentHandle, uint32 AttachmentType, TextureBuildData Data);
		};

		using FrameAttachments = Gfl::Array<FrameTexAttachment>;

		struct FramebufferBuildData
		{
			FrameAttachments Attachments;
			int32 Width;
			int32 Height;

			FramebufferBuildData()  = default;
			~FramebufferBuildData() = default;
		};
	};

	struct GraphicsDriver
	{
		void CreateVertexArray	(GfxHandle& Handle);
		void BindVertexArray	(GfxHandle& Handle);
		void UnbindVertexArray	(GfxHandle& Handle);
		void DeleteVertexArray	(GfxHandle& Handle);

		void CreateBuffer		(GfxHandle& Handle, uint32 Target);
		void BindBuffer			(GfxHandle& Handle);
		void UnbindBuffer		(GfxHandle& Handle);
		void DeleteBuffer		(GfxHandle& Handle);

		void CreateShader		(GfxHandle& Handle, uint32 Type);
		void DeleteShader		(GfxHandle& Handle);

		bool SetUniformBool		(uint32 Location, bool Value);
		bool SetUniformInt		(uint32 Location, int32 Value);
		bool SetUniformFloat	(uint32 Location, float32 Value);
		bool SetUniformDouble	(uint32 Location, float64 Value);
		bool SetUniformVec2F	(uint32 Location, float32* Value);
		bool SetUniformVec2D	(uint32 Location, float64* Value);
		bool SetUniformVec3F	(uint32 Location, float32* Value);
		bool SetUniformVec3D	(uint32 Location, float64* Value);
		bool SetUniformVec4F	(uint32 Location, float32* Value);
		bool SetUniformVec4D	(uint32 Location, float64* Value);
		bool SetUniformMat2F	(uint32 Location, float32* Value);
		bool SetUniformMat2D	(uint32 Location, float64* Value);
		bool SetUniformMat3F	(uint32 Location, float32* Value);
		bool SetUniformMat3D	(uint32 Location, float64* Value);
		bool SetUniformMat4F	(uint32 Location, float32* Value);
		bool SetUniformMat4D	(uint32 Location, float64* Value);

		void CreateProgram		(GfxHandle& Handle);
		void BindProgram		(GfxHandle& Handle);
		void UnbindProgram		(GfxHandle& Handle);
		void DeleteProgram		(GfxHandle& Handle);

		void CreateTexture		(GfxHandle& Handle, uint32 Target);
		void BindTexture		(GfxHandle& Handle);
		void UnbindTexture		(GfxHandle& Handle);
		void DeleteTexture		(GfxHandle& Handle);
		void ActiveTexture		(uint32 Unit);

		void CreateFramebuffer	(GfxHandle& Handle);
		void BindFramebuffer	(GfxHandle& Handle);
		void UnbindFramebuffer	(GfxHandle& Handle);
		void DeleteFramebuffer	(GfxHandle& Handle);

		void BuildMesh			(GfxHandle& Handle, const Driver::MeshBuildData& Data);
		void BuildTexture		(GfxHandle& Handle, const Driver::TextureBuildData& Data);
		bool CompileShader		(GfxHandle& Handle, uint32 Type, const char* SrcCode);
		void BuildShaderProgram	(GfxHandle& Handle, const Driver::ShaderBuildData& Data);
		void BuildFramebuffer	(GfxHandle& Handle, const Driver::FramebufferBuildData& Data);
		void BuildBuffer		(GfxHandle& Handle, const Driver::BufferBuildData& Data);

		void SetViewport		(int32 X, int32 Y, int32 Width, int32 Height);

		int32		GetProgramUniforms	(GfxHandle& Handle);
		int32		GetProgramAttributes(GfxHandle& Handle);
		UniformAttr	GetActiveUniform	(GfxHandle& Handle, uint32 Index);
		VertexAttr	GetActiveAttribute	(GfxHandle& Handle, uint32 Index);
	};

}

#endif // !GRAVITY_API_GRAPHICS_DRIVER