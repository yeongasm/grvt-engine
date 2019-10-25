#pragma once


/**
* Revamp Status: Completed!
*/


/**
* Gravity engine's wrapper for texture types.
* Supports PBR and non-PBR workflow.
*/
enum TextureType : int32 {
	GrvtTexture_Type_None			= 0xFF,	/** On first init only. */
	GrvtTexture_Type_Diffuse		= 0x00,
	GrvtTexture_Type_Albedo			= 0x01,
	GrvtTexture_Type_Specular		= 0x02,
	GrvtTexture_Type_Roughness		= 0x03,
	GrvtTexture_Type_Metallic		= 0x04,
	GrvtTexture_Type_Ao				= 0x05,
	GrvtTexture_Type_Normal			= 0x06,
	GrvtTexture_Type_Height			= 0x07,
	GrvtTexture_Type_Displacement	= 0x08,
	GrvtTexture_Type_Reflection		= 0x09,
	GrvtTexture_Type_Emissive		= 0x0A,
	GrvtTexture_Type_Opacity		= 0x0B,
	GrvtTexture_Type_Cubemap		= 0x0C
};


/**
*/
struct TextureImportInfo {

	Array<String>	Path;
	String			Name;
	TextureType		Type;

};


/**
*/
struct TextureProps {

	String			Path;
	Array<uint8*>	DataPtr;
	int32			Width;
	int32			Height;
	int32			Channel;

};


/**
*/
class GrvtTexture {
public:

	Array<TextureProps> Properties;
	ObjHandle	Handle;
	TextureType Type;

	GrvtTexture();
	~GrvtTexture();

private:

	GrvtTexture* Alloc(const TextureImportInfo& Info);
	void Free();

	GrvtTexture(const GrvtTexture&)				= delete;
	GrvtTexture& operator= (const GrvtTexture&)	= delete;
	
	GrvtTexture(GrvtTexture&&)					= delete;
	GrvtTexture& operator= (GrvtTexture&&)		= delete;

};