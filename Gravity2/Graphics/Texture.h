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
* @param [REQUIRED] (TextureType)	Default = 0;	Type		- Texture's to be used for type. E.g: TEXTURE_TYPE_DIFFUSE, TEXTURE_TYPE_SPECULAR.
* @param [REQUIRED] (String)						Name		- Identification of the specified texture.
* @param [REQUIRED] (String)						Directory	- Directory to the files.
* @param [OPTIONAL] (Files)							Files		- Texture files. Only specify 1 or 6 along with it's face. Any more or any less and Gravity would not generate an OpenGL texture.
*/
struct TextureCreationInfo {
private:

	using TextureFiles = Array<String>;

public:

	TextureType		type;
	String			name;
	String			directory;
	TextureFiles	files;

	TextureCreationInfo();
};


/**
* Generates generic cubemap creation info:
* Alloc functions would override some formats specified by this function.
*
* TextureCreationInfo.target		= GL_TEXTURE_CUBE_MAP;
* TextureCreationInfo.type			= GL_UNSIGNED_BYTE;
* TextureCreationInfo.format		= GL_RGB;
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
	TextureID	id;
	TextureType type;
	TextureData	*info;

	Texture();

	Texture(const Texture &Other)				= delete;
	Texture& operator= (const Texture &Other)	= delete;
	
	Texture(Texture &&Other);
	Texture& operator= (Texture &&Other);
	
	~Texture();

	void Free();
};