#include "stdafx.h"


GravityProject::GravityProject() : 
	Name(), Directory(), ResourceHandle(nullptr) {}


GravityProject::GravityProject(const GravityProject &Other) { *this = Other; }


GravityProject::GravityProject(GravityProject &&Other) { *this = Move(Other); }


GravityProject& GravityProject::operator= (const GravityProject &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Name			= Other.Name;
		Directory		= Other.Directory;
		ResourceHandle	= Other.ResourceHandle;
	}

	return *this;
}


GravityProject& GravityProject::operator= (GravityProject &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Name			= Other.Name;
		Directory		= Other.Directory;
		ResourceHandle	= Other.ResourceHandle;

		new (&Other) GravityProject();
	}

	return *this;
}
  

GravityProject::~GravityProject() { Release(); }


void GravityProject::Release() {
	Name.Release();
	Directory.Release();

	if (ResourceHandle) {
		delete ResourceHandle;
		ResourceHandle = nullptr;
	}
}


void GravityProject::Save() {

}