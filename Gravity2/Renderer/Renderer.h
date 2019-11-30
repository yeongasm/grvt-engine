#pragma once


/**
* GrvtRenderer data structure.
*
* GrvtRenderer is an abstract class that acts like a shell for any custom implemented renderer.
*/
struct GrvtRenderer
{
	GrvtRenderer()			{};
	virtual ~GrvtRenderer() {};

	/**
	* Initialises the renderer. 
	* Called during engine's startup.
	*/
	virtual void Init()		= 0;

	/**
	* Renders data supplied onto the screen.
	* Called every tick.
	*/
	virtual void Render()	= 0;

	/**
	* Releases all resource held by the engine.
	* called during engine's shutdown.
	*/
	virtual void Shutdown() = 0;
};