#include "stdafx.h"


GrvtTexture::GrvtTexture() : 
	Handle(), DataPtr(nullptr), Type(GrvtTexture_Type_None), Width(0), Height(0), Channel(0) {}


GrvtTexture::~GrvtTexture() {}


GrvtTexture* GrvtTexture::Alloc(const TextureImportInfo& Info) 
{
	Type = Info.Type;

	return this;
}


void GrvtTexture::Free() {}