#pragma once


class BasicRenderer : public GrvtRenderer
{
private:
	RenderCache			Cache;
	GrvtCommandBuffer*	CommandBuffer;
	GrvtShader*			ActiveShader;
};