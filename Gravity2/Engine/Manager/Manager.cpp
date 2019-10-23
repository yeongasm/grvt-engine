#include "stdafx.h"


ResId::ResId() : Id(0) {}


ResId::~ResId() { Id = 0; }


ResId::ResId(size_t Id) : Id(Id) {}


EngineResource::EngineResource() :
	References(), ResourcePtr(nullptr), Type(GrvtResource_Type_None), RefCount(0) {}


EngineResource::~EngineResource() {}