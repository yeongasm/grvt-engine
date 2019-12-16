#pragma once

#ifndef GRAVITY_ABSTRACTION_TEXTURE
#define GRAVITY_ABSTRACTION_TEXTURE

#include "Minimal.h"
#include "Framework/Foundation/Foundations.h"

namespace Grvt
{

	/**
	* Revamp Status: Completed!
	*/


	/**
	* Gravity engine's wrapper for texture types.
	* Supports PBR and non-PBR workflow.
	*/
	enum TextureType : int32
	{
		GrvtTexture_Type_None = 0xFF,	/** On first init only. */
		GrvtTexture_Type_Diffuse = 0x00,
		GrvtTexture_Type_Albedo = 0x01,
		GrvtTexture_Type_Specular = 0x02,
		GrvtTexture_Type_Roughness = 0x03,
		GrvtTexture_Type_Metallic = 0x04,
		GrvtTexture_Type_Ao = 0x05,
		GrvtTexture_Type_Normal = 0x06,
		GrvtTexture_Type_Height = 0x07,
		GrvtTexture_Type_Displacement = 0x08,
		GrvtTexture_Type_Reflection = 0x09,
		GrvtTexture_Type_Emissive = 0x0A,
		GrvtTexture_Type_Opacity = 0x0B,
		GrvtTexture_Type_Cubemap = 0x0C
	};


	/**
	*/
	struct TextureImportInfo
	{
		Gfl::Array<Gfl::String>	Path;
		Gfl::String				Name;
		TextureType				Type;
	};


	/**
	*/
	class GrvtTexture
	{
	public:

		union {
			uint8*	DataPtr;
			uint8*	CubemapPtr[6];
		};

		ObjHandle	Handle;
		TextureType Type;
		int32		Width;
		int32		Height;
		int32		Channel;

		GrvtTexture();
		~GrvtTexture();

		GrvtTexture* Alloc(const TextureImportInfo& Info);
		void Free();

	private:


		GrvtTexture(const GrvtTexture&) = delete;
		GrvtTexture& operator= (const GrvtTexture&) = delete;

		GrvtTexture(GrvtTexture&&) = delete;
		GrvtTexture& operator= (GrvtTexture&&) = delete;
	};

}

#endif // !GRAVITY_ABSTRACTION_TEXTURE