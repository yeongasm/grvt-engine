#pragma once


/**
* Types of light.
* To be used upon creating a light light object onto the world.
*/
enum LightType : uint {
	LIGHT_TYPE_NONE			= 0x00,
	LIGHT_TYPE_DIRECTIONAL	= 0x01,
	LIGHT_TYPE_POINTLIGHT	= 0x02
};


/**
* Light creation data structure.
* This struct currently cater towards the Phong/Blinn-Phong lighting model hence this is only temporary.
* In the future, this would be altered for the PBR lighting model.
*
* @param [OPTIONAL] (bool)		Default = true						shadows		- Enable shadow mappign for this light source.
* @param [OPTIONAL] (float)		Default = 0.5						brightness	- Light's brightness.
* @param [OPTIONAL] (float)		Default = 1.0						constant	- Light's constant for calculating point light attenuation.
* @param [OPTIONAL] (float)		Default = 0.0						linear		- Light's linear value for calculating point light attenuation.
* @param [OPTIONAL] (float)		Default = 0.0						quadratic	- Light's quadratic value for calculating point light attenuation.
* @param [OPTIONAL] (float)		Default = 0.0						radius		- Light's radius for calculating point light attenuation.
* @param [REQUIRED] (LightType)										type		- Type of light being created.
* @param [OPTIONAL] (glm::vec3) Default = glm::vec3(1.0, 1.0, 1.0)	lightColour - Colour of light. Default colour is white if nothing is set.
* @param [OPTIONAL] (glm::vec3) Default = glm::vec3(0.0, 0.0, 0.0)	position	- Light's initial position.
* @param [REQUIRED] (String)										name		- Light's identity.
*/
struct LightCreationInfo {
	bool		shadows;
	float		brightness;
	float		constant;
	float		linear;
	float		quadratic;
	float		radius;
	LightType	type;
	glm::vec3	position;
	glm::vec3	lightColour;
	String		name;

	LightCreationInfo();

	LightCreationInfo(const LightCreationInfo &Other);
	LightCreationInfo(LightCreationInfo &&Other);

	LightCreationInfo& operator= (const LightCreationInfo &Other);
	LightCreationInfo& operator= (LightCreationInfo &&Other);

	~LightCreationInfo();
};


/**
* Light data structure.
*
* This is the BASE class for all light types.
* NEVER create an object of this base light data structure.
*
* TODO(Afiq):
* Add framebuffers to enable shadow mapping and include it as part of the structure.
* Add a function to generate debug sphere in wireframe mode.
*/
class Light {
public:
	bool		enable;
	bool		shadows;
	float		brightness;
	LightType	type;
	glm::vec3	position;
	glm::vec3	lightColour;
	String		name;

	Light();

	Light(const Light &Other);
	Light(Light &&Other);

	Light& operator= (const Light &Other);
	Light& operator= (Light &&Other);

	virtual ~Light();

	void			Alloc	(const LightCreationInfo &Info);
	void			Free	();

	virtual void	Compute	(glm::mat4 &Buffer);
};


/**
* Directional light data structure.
* 
* We could easily just make the base Light object a directional light but it wouldn't seem proper in programming terms.
* Compute method computes information required for directional light.
*/
class DirLight : public Light {
public:

	DirLight();

	DirLight(const DirLight &Other);
	DirLight(DirLight &&Other);

	DirLight& operator= (const DirLight &Other);
	DirLight& operator= (DirLight &&Other);

	~DirLight();
};


/**
* Point Light data structure.
* Compute() method is overridden in this structure and is used to pass in data for Point Light calculations.
*
* TODO(Afiq):
* Add framebuffers to enable shadow mapping and include it as part of the structure.
* Add a function to generate debug sphere in wireframe mode.
*/
class PointLight : public Light {
private:

	bool simplified;

public:
	/**
	* NOTE(Afiq):
	* For now we manually set in the values for the constants, linear and quadratic.
	* In the future, we adjust these values base on the assigned radius.
	*/
	float constant;
	float linear;
	float quadratic;
	float radius;

	PointLight();

	PointLight(const PointLight &Other);
	PointLight(PointLight &&Other);

	PointLight& operator= (const PointLight &Other);
	PointLight& operator= (PointLight &&Other);

	~PointLight();

	void		Alloc					(const LightCreationInfo &Info);
	void		Free					();
	void		UseRadiusForAttenuation	(bool Enable = true);

	void		Compute					(glm::mat4 &Buffer);
};


/**
* TODO(Afiq):
* In the future when there will be shadow mapping, we'll need to think of a way to store the light space matrix and
* the depth map inside of these Render(Directional/Point)Light objects.
*/


/**
* RenderDirectionalLight data structure.
*
* Only to be used inside the RenderBuffer object. Under no circumstances shall anyone make an instantiation of this object.
*/
struct RenderDirectionalLight {
	float		brightness;
	glm::vec3	position;
	glm::vec3	colour;

	RenderDirectionalLight();

	RenderDirectionalLight(const RenderDirectionalLight &Other)				= delete;
	RenderDirectionalLight(RenderDirectionalLight &&Other)					= delete;

	RenderDirectionalLight& operator= (const RenderDirectionalLight &Other) = delete;
	RenderDirectionalLight& operator= (RenderDirectionalLight &&Other)		= delete;

	~RenderDirectionalLight();
};


/**
* RenderPointLight data structure.
*
* Only to be used inside the RenderBuffer object. Under no circumstances shall anyone make an instantiation of this object.
*/
struct RenderPointLight {
	Array<float>		brightness;
	Array<float>		constant;
	Array<float>		linear;
	Array<float>		quadratic;
	Array<glm::vec3>	position;
	Array<glm::vec3>	colour;

	RenderPointLight();

	RenderPointLight(const RenderPointLight &Other)				= delete;
	RenderPointLight(RenderPointLight &&Other)					= delete;

	RenderPointLight& operator= (const RenderPointLight &Other) = delete;
	RenderPointLight& operator= (RenderPointLight &&Other)		= delete;

	~RenderPointLight();
};