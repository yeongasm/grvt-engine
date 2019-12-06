#pragma once
#ifndef GRAVITY_CORE_APPLICATION
#define GRAVITY_CORE_APPLICATION

namespace Grvt
{

	class GrvtEngine;


	class GrvtApplication
	{
	protected:

		friend class GrvtEngine;

		GrvtEngine* Engine;

	public:

		GrvtApplication();
		~GrvtApplication();

		GrvtApplication(const GrvtApplication& Other) = delete;
		GrvtApplication& operator= (const GrvtApplication& Other) = delete;

		GrvtApplication(GrvtApplication&& Other) = delete;
		GrvtApplication& operator= (GrvtApplication&& Other) = delete;

		virtual void Initialise(GrvtEngine* EnginePtr);
		virtual void Tick();
		virtual void Shutdown();
	};

}

#endif // !GRAVITY_CORE_APPLICATION