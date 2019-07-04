#pragma once


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
	using Instances = Array<SceneInstance*>;

	bool			hasDirLight;
	DirLight*		dirLight;
	Array<Light*>	lights;
	Instances		renderInstances;

	size_t			CheckLightName			(const String &Name);

	friend class Renderer;
public:
	String name;

	Scenery();

	Scenery(const Scenery &Other);
	Scenery(Scenery &&Other);

	Scenery& operator= (const Scenery &Other);
	Scenery& operator= (Scenery &&Other);

	~Scenery();

	void			Free					();

	void			PushSceneInstance		(SceneInstance *Instance);
	void			GetInstancesOfScene		(Scene *Scene, Array<SceneInstance*> &Buffer);
	
	SceneInstance*	CreateSceneInstance		(Scene *Scene, const SceneInstanceCreation &Info);
	DirLight*		AddDirectionalLight		(const LightCreationInfo &Info);
	PointLight*		AddPointLight			(const LightCreationInfo &Info);
	
	bool			PopSceneInstance		(const SceneInstance *Instance);
	bool			PopAllInstanceWithScene	(const Scene *Scene);
	bool			PopLight				(const String &Name);
};