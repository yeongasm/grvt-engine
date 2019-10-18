#include "stdafx.h"


TextureCreationInfo::TextureCreationInfo() : 
	Name(), Path(), Type(TEXTURE_TYPE_NONE) {}


TextureCreationInfo::~TextureCreationInfo() {
	Name.Release();
	Path.Release();
}


TextureObj::TextureObj() : Handle(), Type(TEXTURE_TYPE_NONE), Info(nullptr) {}


TextureObj::~TextureObj() {}