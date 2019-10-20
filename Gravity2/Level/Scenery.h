#pragma once


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
class GravityLevel {
private:

	DirLight			*DirLight;
	EulerCamera			*Camera;
	Array<LightSource*>	Lights;

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