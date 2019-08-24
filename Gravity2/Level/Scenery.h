#pragma once


/**
* Scenery creation data structure.
*
* Provide information to create a level inside of the engine.
* A unique name must be provided inside of the object else creation would fail.
*
* @param [REQUIRED] (String) Name		- The level's name for identification.
* @param [OPTIONAL] (String) Directory	- If specified, the engine would place the level's JSON file in the specified directory.
* @param [OPTIONAL] (String) Filename	- If specified, the engine would save the level's JSON file with this name instead of using Name.
*/
struct LevelCreationInfo {
public:
	String name;
	String directory;
	String filename;

	LevelCreationInfo();
	LevelCreationInfo(const LevelCreationInfo &Other);
	LevelCreationInfo(LevelCreationInfo &&Other);

	LevelCreationInfo& operator= (const LevelCreationInfo &Other);
	LevelCreationInfo& operator= (LevelCreationInfo &&Other);

	~LevelCreationInfo();
};


struct SceneryData;


/**
* Gravity's Scenery data structure.
*
* A Scenery is typically a level. Everything inside of a Scenery will be passed into the renderer every frame to be rendered.
* Since Gravity is only meant to be a rendering engine, we do not take into account dynamically rendered objects.
*
* TODO(Afiq):
* Expand saving into JSON file capabilities to save and load a scenery.
*/
class Scenery {
private:

	using Instances = SceneInstance*;

	bool				hasDirLight;
	DirLight			*dirLight;
	EulerCamera			*camera;
	Array<Light*>		lights;
	Array<Instances>	renderInstances;

	size_t			CheckLightName			(const String &Name);

	friend class Renderer;

public:

	SceneryData *info;

	Scenery();

	Scenery(const Scenery &Other);
	Scenery(Scenery &&Other);

	Scenery& operator= (const Scenery &Other);
	Scenery& operator= (Scenery &&Other);

	~Scenery();

	void			Free					();
	void			PushSceneInstance		(SceneInstance *Instance);
	void			AttachCamera			(EulerCamera *Camera);
	void			DetachCamera			();

	SceneInstance*	CreateSceneInstance		(Scene *Scene, const SceneInstanceCreation &Info);
	DirLight*		AddDirectionalLight		(const LightCreationInfo &Info);
	PointLight*		AddPointLight			(const LightCreationInfo &Info);

	DirLight*		GetDirectionalLight		();
	PointLight*		GetPointLight			(const String &Name);
	
	bool			PopSceneInstance		(const SceneInstance *Instance);
	bool			PopAllInstanceWithScene	(const Scene *Scene);
	bool			PopLight				(const String &Name);
};