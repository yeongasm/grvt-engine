#include "stdafx.h"


ProjectCreationInfo::ProjectCreationInfo() : name{}, filename{}, directory{} {}


ProjectCreationInfo::ProjectCreationInfo(const ProjectCreationInfo &Other) { *this = Other; }


ProjectCreationInfo::ProjectCreationInfo(ProjectCreationInfo &&Other) { *this = std::move(Other); }


ProjectCreationInfo& ProjectCreationInfo::operator= (const ProjectCreationInfo &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name		= Other.name;
		filename	= Other.filename;
		directory	= Other.directory;
	}

	return *this;
}


ProjectCreationInfo& ProjectCreationInfo::operator= (ProjectCreationInfo &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name		= Other.name;
		filename	= Other.filename;
		directory	= Other.directory;

		new (&Other) ProjectCreationInfo();
	}

	return *this;
}


ProjectCreationInfo::~ProjectCreationInfo() {
	name.Release();
	filename.Release();
	directory.Release();
}


GravityProject::GravityProject() : name(), filename(), directory(), resourceHandler(nullptr) {}


GravityProject::GravityProject(const GravityProject &Other) { *this = Other; }


GravityProject::GravityProject(GravityProject &&Other) { *this = std::move(Other); }


GravityProject& GravityProject::operator= (const GravityProject &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name			= Other.name;
		filename		= Other.filename;
		directory		= Other.directory;
		resourceHandler = Other.resourceHandler;
	}

	return *this;
}


GravityProject& GravityProject::operator= (GravityProject &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name			= Other.name;
		filename		= Other.filename;
		directory		= Other.directory;
		resourceHandler = Other.resourceHandler;
	}

	return *this;
}
  

GravityProject::~GravityProject() { Release(); }


void GravityProject::Release() {
	name.Release();
	filename.Release();
	directory.Release();

	if (resourceHandler) {
		delete resourceHandler;
		resourceHandler = nullptr;
	}
}


void GravityProject::Alloc(const ProjectCreationInfo &Info) {
	name		= Info.name;
	filename	= Info.filename;
	directory	= Info.directory;

	if (!resourceHandler)
		resourceHandler = new ResourceManager();
}


void GravityProject::Save() {

}