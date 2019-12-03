#pragma once


class ENGINE_API BasicRenderer : public GrvtRenderer
{
private:
	RenderCache			Cache;
	GrvtCommandBuffer*	CommandBuffer;
	GrvtShader*			ActiveShader;
};