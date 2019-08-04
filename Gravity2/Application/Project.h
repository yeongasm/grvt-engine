#pragma once


/**
* Project creation data structure.
*
* @param [REQUIRED] (String) name		- Name of project.
* @param [OPTIONAL] (String) filename	- Name of the project file. Defaults to having the same name as the project.
* @param [OPTIONAL] (String) directory	- Directory where the project file will be saved. 
*/
struct ProjectCreationInfo {
	String name;
	String filename;
	String directory;

	ProjectCreationInfo();

	ProjectCreationInfo(const ProjectCreationInfo &Other);
	ProjectCreationInfo(ProjectCreationInfo &&Other);

	ProjectCreationInfo& operator= (const ProjectCreationInfo &Other);
	ProjectCreationInfo& operator= (ProjectCreationInfo &&Other);

	~ProjectCreationInfo();
};


/**
* Gravity Project data structure.
*
* A project encompassed all the details required to build a scene.
*/
class GravityProject {
private:

	ResourceManager	*resourceHandler;

	/**
	* Release all the contents of this project.
	*/
	void Release();

	friend class GravityApp;
public:

	String			name;
	String			filename;
	String			directory;
	
	GravityProject();
	
	GravityProject(const GravityProject &Other);
	GravityProject(GravityProject &&Other);

	GravityProject& operator= (const GravityProject &Other);
	GravityProject& operator= (GravityProject &&Other);

	~GravityProject();

	/**
	* Creates a new project by specifying necessary information.
	*/
	void Alloc(const ProjectCreationInfo &Info);

	/**
	* Save the current state of the project.
	*/
	void Save();
};