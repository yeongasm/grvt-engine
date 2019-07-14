#pragma once


/**
* RenderBuffer data structure.
*
* The idea of a RenderBuffer is to seperate the Renderer from the Application's sequential pipeline.
* RenderBuffer only acts as a container to store data and does not manipulate data in any way.
* As of 13.07.2019, multi-threading has not been implemented into the engine.
* The idea is to make the Renderer it's stand-alone "application", running in a separate thread.
* GPU would be constantly busy rendering from the buffers without caring too much about lag from the CPU.
*/
class RenderBuffer {
private:

	using Command = RenderCommand;

public:

	glm::mat4				compProjView;
	Array<glm::mat4>		lights;
	Array<Command>			commands;
	//Array<Command>			alphaCommands;
	//Array<Command>			instancingCommands;
	EulerCamera				*camera;

	

	/**
	* TODO(Afiq):
	* Create data structures for shadow maps & framebuffers as well? 
	*/

	RenderBuffer();

	RenderBuffer(const RenderBuffer &Other);
	RenderBuffer(RenderBuffer &&Other);

	RenderBuffer& operator= (const RenderBuffer &Other);
	RenderBuffer& operator= (RenderBuffer &&Other);

	~RenderBuffer();

	void Free();
};