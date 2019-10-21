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
class RenderInstance {
private:

	using Command = RenderCommand;

public:

	glm::mat4				compProjView;
	Array<glm::mat4>		lights;
	Array<Command>			commands;
	//Array<Command>			instancingCommands;
	EulerCamera				*camera;

	RenderInstance();

	RenderInstance(const RenderInstance &Other);
	RenderInstance(RenderInstance &&Other);

	RenderInstance& operator= (const RenderInstance &Other);
	RenderInstance& operator= (RenderInstance &&Other);

	~RenderInstance();

	void Free();
};