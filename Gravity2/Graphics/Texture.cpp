#include "stdafx.h"

Texture::Texture() : id{}, target{}, type(TEXTURE_TYPE_NONE), info{} {}

Texture::Texture(const TextureCreationInfo &Info) : Texture() {
	Alloc(Info);
}

//Texture::Texture(const Texture &Other) { *this = Other; }

Texture::Texture(Texture &&Other) { *this = std::move(Other); }

//Texture& Texture::operator= (const Texture &Other) {
//	id		= Other.id;
//	target	= Other.target;
//	type	= Other.type;
//	name	= Other.name;
//
//	return *this;
//}

Texture& Texture::operator= (Texture &&Other) {
	_ASSERTE(this != &Other);

	Free();

	id		= Other.id;
	target	= Other.target;
	type	= Other.type;
	info	= Other.info;

	Other.Free();

	return *this;
}

Texture::~Texture() {
	Free();
}

bool Texture::Alloc(const TextureCreationInfo &Info) {
	bool emptyTexture = true;
	Array<Pair<unsigned char*, GLenum>> textureData;

	Array<String> filePaths;
	for (String &file : Info.files) {
		filePaths.Push(String("%s/%s", ~Info.directory, ~file));
	}

	if (filePaths.Length()) {
		if (filePaths.Length() > 1 && filePaths.Length() < 6)
			return false;

		int nrChannels;

		textureData.Reserve(6);
		for (uint i = 0; i < filePaths.Length(); i++) {
			GLenum format;

			stbi_set_flip_vertically_on_load(Info.flip);

			textureData[i].Key = stbi_load(~filePaths[i], (int*)&Info.width, (int*)&Info.height, &nrChannels, 0);

			if (!textureData[i].Key)
				return false;

			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			textureData[i].Value = format;
		}

		emptyTexture = false;
	}
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	if ((Info.cubemap || Info.target == GL_TEXTURE_CUBE_MAP) && !emptyTexture) {
		for (uint i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureData[i].Value, Info.width, Info.height, 0, textureData[i].Value, Info.type, textureData[i].Key);

	} else {
		GLenum format		= (emptyTexture) ? Info.format	: textureData[0].Value;
		unsigned char *data = (emptyTexture) ? nullptr		: textureData[0].Key;

		glTexImage2D(Info.target, 0, format, Info.width, Info.height, 0, format, Info.type, data);

		if (Info.mipmap)
			glGenerateMipmap(Info.target);
		
	}

	for (Pair<uint, uint> &Params : Info.parameters)
		glTexParameteri(Info.target, Params.Key, Params.Value);
	

	if (!emptyTexture)
		for (uint i = 0; i < textureData.Length(); i++)
			stbi_image_free(textureData[i].Key);

	glBindTexture(Info.target, 0);

	target	= Info.target;
	type	= Info.textureType;

	return true;
}

void Texture::Free() {
	glDeleteTextures(1, &id);
	type	= TEXTURE_TYPE_NONE;
	target	= 0;
}

void GenerateGenericTextureInfo(TextureCreationInfo &Info) {
	Info.target			= GL_TEXTURE_2D;
	Info.type			= GL_UNSIGNED_BYTE;
	Info.format			= GL_RGB;
	Info.parameters.Push({GL_TEXTURE_WRAP_S, GL_REPEAT});
	Info.parameters.Push({GL_TEXTURE_WRAP_T, GL_REPEAT});
	Info.parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR});
	Info.parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});
}

void GenerateGenericCubemapInfo(TextureCreationInfo &Info) {
	Info.target		= GL_TEXTURE_CUBE_MAP;
	Info.type		= GL_UNSIGNED_BYTE;
	Info.format		= GL_RGB;
	Info.parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
	Info.parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});
	Info.parameters.Push({GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE});
	Info.parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
	Info.parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});
}
