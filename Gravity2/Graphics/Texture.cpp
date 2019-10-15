#include "stdafx.h"


TextureObj::TextureObj() : Handle(), Type(TEXTURE_TYPE_NONE), Info() {}


TextureObj::~TextureObj() {}


TextureCreationInfo::TextureCreationInfo() : Name(), Path(), Type(TEXTURE_TYPE_NONE) {}