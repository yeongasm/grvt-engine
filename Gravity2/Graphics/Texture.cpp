#include "stdafx.h"


Texture::Texture() : handle(), type(TEXTURE_TYPE_NONE), info() {}


Texture::Texture(Texture &&Other) { *this = std::move(Other); }


Texture& Texture::operator= (Texture &&Other) {
	_ASSERTE(this != &Other);

	Free();

	handle	= std::move(Other.handle);
	type	= Other.type;
	info	= Other.info;

	Other.Free();

	return *this;
}


Texture::~Texture() {
	Free();
}


void Texture::Free() {
	type = TEXTURE_TYPE_NONE;
}


TextureCreationInfo::TextureCreationInfo() : type(TEXTURE_TYPE_NONE), directory(), name(), files() {}