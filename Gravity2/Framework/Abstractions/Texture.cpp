#include "stdafx.h"


TextureProps::TextureProps() :
	Path(), DataPtr(nullptr), Width(0), Height(0), Channel(0) {}


TextureProps::~TextureProps() {}


GrvtTexture::GrvtTexture() : 
	Properties(), Handle(), Type(GrvtTexture_Type_None) {}


GrvtTexture::~GrvtTexture() {}


GrvtTexture* GrvtTexture::Alloc(const TextureImportInfo& Info) {
	Type = Info.Type;
	for (const std::string& path : Info.Path) {
		TextureProps& prop = Properties.Insert(TextureProps());
		prop.Path = path;
	}

	return this;
}


void GrvtTexture::Free() {
	for (TextureProps& prop : Properties) {
		prop.Path.clear();
		delete prop.DataPtr;
	}
}