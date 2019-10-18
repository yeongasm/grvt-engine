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
* @param [OPTIONAL] (glm::vec3) Default = glm::vec3(0.0, 0.0, 0.0)	Position	- Light's initial position.
* @param [OPTIONAL] (glm::vec3) Default = glm::vec3(1.0, 1.0, 1.0)	Colour		- Colour of light. Default colour is white if nothing is set.
* @param [REQUIRED] (String)										Name		- Light's identity.
* @param [REQUIRED] (LightType)										Type		- Type of light being created.
* @param [OPTIONAL] (float)		Default = 0.5						Brightness	- Light's brightness.
* @param [OPTIONAL] (float)		Default = 1.0						Constant	- Light's constant for calculating point light attenuation.
* @param [OPTIONAL] (float)		Default = 0.0						Linear		- Light's linear value for calculating point light attenuation.
* @param [OPTIONAL] (float)		Default = 0.0						Quadratic	- Light's quadratic value for calculating point light attenuation.
* @param [OPTIONAL] (float)		Default = 0.0						Radius		- Light's radius for calculating point light attenuation.
*/
struct LightCreationInfo {

	glm::vec3	Position;
	glm::vec3	Colour;
	String		Name;
	LightType	Type;
	float		Brightness;
	float		Constant;
	float		Linear;
	float		Quadratic;
	float		Radius;

	LightCreationInfo();
	~LightCreationInfo();

	LightCreationInfo(const LightCreationInfo &Other);
	LightCreationInfo& operator= (const LightCreationInfo &Other);

	LightCreationInfo(LightCreationInfo &&Other);
	LightCreationInfo& operator= (LightCreationInfo &&Other);

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
* Figure out a way to send lighting information over to the shader without using a uniform.
*/
class LightSource {
public:

	glm::vec3	Position;
	glm::vec3	Colour;
	String		Name;
	LightType	Type;
	float		Brightness;
	bool		Enable;

	LightSource();
	virtual ~LightSource();

	LightSource(const LightSource &Other);
	LightSource& operator= (const LightSource &Other);

	LightSource(LightSource &&Other);
	LightSource& operator= (LightSource &&Other);

	virtual void	Alloc	(const LightCreationInfo &Info) = 0;
	virtual void	Free	()								= 0;
	virtual void	Compute	(glm::mat4 &Buffer)				= 0;

};


/**
* Directional light data structure.
* 
* We could easily just make the base Light object a directional light but it wouldn't seem proper in programming terms.
* Compute method computes information required for directional light.
*/
class DirLight : public LightSource {
public:

	DirLight();
	~DirLight();

	DirLight(const DirLight &Other);
	DirLight& operator= (const DirLight &Other);

	DirLight(DirLight &&Other);
	DirLight& operator= (DirLight &&Other);

	void	Alloc	(const LightCreationInfo &Info);
	void	Free	();
	void	Compute	(glm::mat4 &Buffer);

};


/**
* Point Light data structure.
* Compute() method is overridden in this structure and is used to pass in data for Point Light calculations.
*
* TODO(Afiq):
* Add framebuffers to enable shadow mapping and include it as part of the structure.
* Add a function to generate debug sphere in wireframe mode.
*/
class PointLight : public LightSource {
public:

	/**
	* NOTE(Afiq):
	* For now we manually set in the values for the constants, linear and quadratic.
	* In the future, we adjust these values base on the assigned radius.
	*/
	float	Constant;
	float	Linear;
	float	Quadratic;
	float	Radius;
	bool	Simplified;

	PointLight();
	~PointLight();

	PointLight(const PointLight &Other);
	PointLight& operator= (const PointLight &Other);

	PointLight(PointLight &&Other);
	PointLight& operator= (PointLight &&Other);

	void	Alloc					(const LightCreationInfo &Info);
	void	Free					();
	void	UseRadiusForAttenuation	(bool Enable = true);

	void	Compute					(glm::mat4 &Buffer);

};