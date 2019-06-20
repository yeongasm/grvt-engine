#pragma once


/**
* Types of light.
* To be used upon creating a light light object onto the world.
*/
enum LightType : uint {
	LIGHT_TYPE_NONE			= 0x00,
	LIGHT_TYPE_DIRECTIONAL	= 0x01,
	LIGHT_TYPE_SPOTLIGHT	= 0x02,
	LIGHT_TYPE_POINTLIGHT	= 0x03
};


/**
* Light creation data structure.
* This struct currently cater towards the Phong/Blinn-Phong lighting model hence this is only temporary.
* In the future, this would be altered for the PBR lighting model.
*
* @param [OPTIONAL] (float)		Default = 0.5						brightness	- Light's brightness.
* @param [REQUIRED] (LightType)										type		- Type of light being created.
* @param [OPTIONAL] (glm::vec3) Default = glm::vec3(1.0, 1.0, 1.0)	lightColour - Colour of light. Default colour is white if nothing is set.
* @param [OPTIONAL] (glm::vec3) Default = glm::vec3(0.0, 0.0, 0.0)	position	- Light's initial position.
* @param [REQUIRED] (String)										name		- Light's identity.
*/
struct LightCreationInfo {
	float		brightness;
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
* Default Compute() method passes data into the shader for Directional Lights calculations.
*
* TODO(Afiq):
* Add framebuffers to enable shadow mapping and include it as part of the structure.
* Add a function to generate debug sphere in wireframe mode.
*/
class Light {
public:
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

	/**
	* The base class for Light object's passes in and computes information for Directional Lights only.
	* To compute data for other types of light, it is required to make an instance of the derived light classes.
	*
	* @param [REQUIRED]		(Shader)	Shader - Shader to passed in inside of the rendering stage.
	*/
	virtual void	Compute	(Shader *Shader);
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
};