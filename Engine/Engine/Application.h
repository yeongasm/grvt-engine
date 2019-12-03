#pragma once


class GrvtEngine;


class ENGINE_API GrvtApplication
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