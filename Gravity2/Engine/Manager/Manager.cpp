#include "stdafx.h"


static ResourceManager<GrvtModel> ModelStore;


ResourceType ResourceHandler::GetResourceType(const String& Identifier) {
	auto it = Resources.find(Identifier);
	if (it == Resources.end())
		return GrvtResource_Type_None;

	size_t type = it->second >> (sizeof(size_t) * 8 - 4);
	return *(ResourceType*)&type;
}


GrvtModel* ResourceHandler::NewImportModel(const ModelImportInfo& Import) {
	auto it = Resources.find(Import.Name);
	if (it != Resources.end())
		return ModelStore.Store[it->second].ResourcePtr;

	size_t id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
	Resources.insert({Import.Name, id});

	EngineResource<GrvtModel> model;
	model.Name = Import.Name;
	model.ResourcePtr = new GrvtModel();
	ModelStore.Store.insert({id, model});

	return model.ResourcePtr;
}