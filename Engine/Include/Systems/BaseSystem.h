#pragma once

#ifndef GRAVITY_BASE_SYSTEM
#define GRAVITY_BASE_SYSTEM

namespace Grvt
{

#define GRSYSTEMCRED(System)	#System

#define REGISTERSYSTEM(System)	Name = #System;

	/**
	* The basis of all the systems in the engine.
	* Every system has to be derived from this class.
	* When creating a new system, it is essential to ensure that the systems are already registered inside of the engine before the start of the application loop.
	*/
	class BaseSystem
	{
	public:

		Gfl::String Name;

		BaseSystem() : Name() {};
		virtual ~BaseSystem() {};

		virtual void Init()		= 0;
		virtual void Tick()		= 0;
		virtual void Shutdown()	= 0;
	};

}

#endif // !GRAVITY_BASE_SYSTEM