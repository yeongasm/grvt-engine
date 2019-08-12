#pragma once


/**
* Gravity engine's wrapper for texture types.
* Supports PBR and non-PBR workflow.
* Texture types are also used as texture sampling units to be used inside the shader.
*/
enum TextureType : int {
	TEXTURE_TYPE_NONE			= 0x00,	// On first initialisation only.
	TEXTURE_TYPE_DIFFUSE		= 0x01,
	TEXTURE_TYPE_ALBEDO			= 0x02,
	TEXTURE_TYPE_SPECULAR		= 0x03,
	TEXTURE_TYPE_ROUGHNESS		= 0x04,
	TEXTURE_TYPE_METALLIC		= 0x05,
	TEXTURE_TYPE_AO				= 0x06,
	TEXTURE_TYPE_NORMAL			= 0x07,
	TEXTURE_TYPE_HEIGHT			= 0x08,
	TEXTURE_TYPE_DISPLACEMENT	= 0x09,
	TEXTURE_TYPE_REFLECTION		= 0x0A,
	TEXTURE_TYPE_EMISSIVE		= 0x0B,
	TEXTURE_TYPE_OPACITY		= 0x0C,
	TEXTURE_TYPE_CUBEMAP		= 0x0D,
	TEXTURE_TYPE_FRAMEBFFR		= 0x0E,
	TEXTURE_TYPE_SHADOWMAP		= 0x0F,
	TEXTURE_TYPE_SHADOWCUBEMAP	= 0x10
};


/**
* Gravity engine's cubemap texture face specifier.
* This is mainly used for the convenience of debugging only.
* TextureFace does not affect the order of cubemap generation inside Gravity. And would always generate in the specified order:
* Right -> Left -> Top -> Bottom -> Front -> Back.
*/
enum TextureFace {
	TEXTURE_FACE_NONE = 0,
	TEXTURE_FACE_RIGHT,
	TEXTURE_FACE_LEFT,
	TEXTURE_FACE_TOP,
	TEXTURE_FACE_BOTTOM,
	TEXTURE_FACE_FRONT,
	TEXTURE_FACE_BACK
};


/**
* Texture creation data structure.
* All textures created inside Gravity requires initialising values from this struct.
* All textures created require a name for identification. Failing to provide a name would cause Gravity to abort it's texture generation process.
* If no files are specified, Gravity would just create an empty texture.
* NOTE: No error checks are done in case if the values are mismatched to OpenGL.
* NOTE: Texture generation does not check if you have specified the texture's type. Failing to provide one will mess up the texture's sampling unit for shader use.
* ALERT: When specifying files for a cube map, STRICTLY specify in this order: Right -> Left -> Top -> Bottom -> Front -> Back.
*
* @param [REQUIRED] (bool)			Default = 1;	mipmap	- Generate mipmaps for OpenGL texture.
* @param [OPTIONAL] (bool)			Default = 0;	cubemap - Setting this to true would make the texture into a cubemap texture.
* @param [OPTIONAL] (bool)			Default = 1;	flip	- Flips the image and makes the 0.0 coordinate on the Y-axis be on the bottom side.
* @param [OPTIONAL] (int)			Default = 0;	width	- Texture's width. Use only when creating an empty texture.
* @param [OPTIONAL] (int)			Default = 0;	height	- Texture's height. Use only when creating an empty texture.
* @param [REQUIRED] (uint)			Default = 0;	target	- Texture's target. E.g: GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP.
* @param [REQUIRED] (uint)			Default = 0;	type	- Data type of the pixel. E.g: GL_UNSIGNED_BYTE, GL_UNSIGNED_INT.
* @param [OPTIONAL] (uint)			Default = 0;	format	- Format of the pixel data. If unspecified, Gravity would generate one base on the image's pixel data. E.g: GL_RED, GL_RGB.
* @param [REQUIRED] (TextureType)	Default = 0;	textureType - Texture's to be used for type. E.g: TEXTURE_TYPE_DIFFUSE, TEXTURE_TYPE_SPECULAR.
* @param [REQUIRED] (Params)						parameters	- Texture's parameters. Specify in pairs using Pair() data structure or using initializer lists.
* @param [OPTIONAL] (Files)							files		- Texture files. Only specify 1 or 6 along with it's face. Any more or any less and Gravity would not generate an OpenGL texture.
* @param [REQUIRED] (String)						directory	- Directory to the files.
* @param [REQUIRED] (String)						name		- Identification of the specified texture.
*/
struct TextureCreationInfo {
	typedef Array<Pair<uint, uint>>		Params;
	typedef Array<String>				Files;

	bool		mipmap;
	bool		cubemap;
	bool		flip;
	int			width;
	int			height;
	uint		target;
	uint		type;
	uint		format;
	TextureType textureType;
	Params		parameters;
	Files		files;
	String		directory;
	String		name;

	TextureCreationInfo() : mipmap(1), cubemap(0), flip(1), width(0), height(0), 
		target(0), type(0), format(0), textureType(TEXTURE_TYPE_NONE) {}
};

/**
* Generates generic texture creation info:
* Alloc functions would override some formats specified by this function.
*
* TextureCreationInfo.target		= GL_TEXTURE_2D;
* TextureCreationInfo.type			= GL_UNSIGNED_BYTE;
* TextureCreationInfo.target		= GL_RGB;
* TextureCreationInfo.parameters[0] = {GL_TEXTURE_WRAP_S, GL_REPEAT}
* TextureCreationInfo.parameters[1] = {GL_TEXTURE_WRAP_T, GL_REPEAT}
* TextureCreationInfo.parameters[2] = {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR}
* TextureCreationInfo.parameters[3] = {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
*/
void GenerateGenericTextureInfo(TextureCreationInfo &Info);


/**
* Generates generic cubemap creation info:
* Alloc functions would override some formats specified by this function.
*
* TextureCreationInfo.target		= GL_TEXTURE_CUBE_MAP;
* TextureCreationInfo.type			= GL_UNSIGNED_BYTE;
* TextureCreationInfo.target		= GL_RGB;
* TextureCreationInfo.parameters[0] = {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}
* TextureCreationInfo.parameters[1] = {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}
* TextureCreationInfo.parameters[1] = {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}
* TextureCreationInfo.parameters[2] = {GL_TEXTURE_MIN_FILTER, GL_LINEAR}
* TextureCreationInfo.parameters[3] = {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
*/
void GenerateGenericCubemapInfo(TextureCreationInfo &Info);


// Forward declaration of the struct.
struct TextureData;


/**
* Gravity engine's texture data structure.
* All of Gravity's texture objects are to be of pointer-type.
* When creating a cubemap texture, specify in the order of Right -> Left -> Top -> Bottom -> Front -> Back inside TextureCreationInfo struct.
* Gravity does not take into account the order of faces that is specified in the struct and would always generate in that order.
*/
class Texture {
public:
	uint		id;
	uint		target;
	TextureType type;
	TextureData	*info;

	Texture();
	Texture(const TextureCreationInfo &Info);
	Texture(const Texture &Other)				= delete;
	Texture& operator= (const Texture &Other)	= delete;
	Texture(Texture &&Other);
	Texture& operator= (Texture &&Other);
	~Texture();


	bool Alloc(const TextureCreationInfo &Info);
	void Free();
};