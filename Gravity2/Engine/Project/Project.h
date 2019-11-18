#pragma once


/**
* Gravity Project data structure.
*/
class GravityProject {
private:

	ResourceHandler	*ResourceHandle;

	/**
	* Release all the contents of this project.
	*/
	void Release();

	friend class GravityApp;

public:

	String			Name;
	String			Directory;
	
	GravityProject();
	
	GravityProject(const GravityProject &Other);
	GravityProject(GravityProject &&Other);

	GravityProject& operator= (const GravityProject &Other);
	GravityProject& operator= (GravityProject &&Other);

	~GravityProject();

	/**
	* Save the current state of the project.
	*/
	void Save();
};