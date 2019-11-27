#include "stdafx.h"


FrameStatistics::FrameStatistics() :
	framerateSecPerFrameIdx{},
	framerateSecPerFrameAccum{},
	framerateSecPerFrame{} {}


FrameStatistics::FrameStatistics(const FrameStatistics &Other) { *this = Other; }


FrameStatistics::FrameStatistics(FrameStatistics &&Other) { *this = std::move(Other); }


FrameStatistics& FrameStatistics::operator= (const FrameStatistics &Other) {
	// Make sure that we don't copy to our own self!
	_ASSERTE(this != &Other);

	// Still, we need to make sure we don't actually copy to ourself.
	if (this != &Other) {
		framerateSecPerFrameIdx		= Other.framerateSecPerFrameIdx;
		framerateSecPerFrameAccum	= Other.framerateSecPerFrameAccum;
		memcpy(framerateSecPerFrame, Other.framerateSecPerFrame, GRVT_ARRAY_LENGTH(framerateSecPerFrame));
	}

	return *this;
}


FrameStatistics& FrameStatistics::operator= (FrameStatistics &&Other) {
	// Make sure that we don't do move semantics to our own self!
	_ASSERTE(this != &Other);

	// Still, we need to make sure we don't actually do move semantics to ourself.
	if (this != &Other) {
		framerateSecPerFrameIdx		= Other.framerateSecPerFrameIdx;
		framerateSecPerFrameAccum	= Other.framerateSecPerFrameAccum;
		memcpy(framerateSecPerFrame, Other.framerateSecPerFrame, GRVT_ARRAY_LENGTH(framerateSecPerFrame));
		//CopyRawArray(framerateSecPerFrame, Other.framerateSecPerFrame);

		new (&Other) FrameStatistics();
	}

	return *this;
}


FrameStatistics::~FrameStatistics() {
	framerateSecPerFrameIdx = 0;

	framerateSecPerFrameAccum = 0.0f;

	for (float &data : framerateSecPerFrame)
		data = 0.0f;
}


void FrameStatistics::Tick(float DeltaTime) {
	framerateSecPerFrameAccum += DeltaTime - framerateSecPerFrame[framerateSecPerFrameIdx];
	framerateSecPerFrame[framerateSecPerFrameIdx] = DeltaTime;
	framerateSecPerFrameIdx = (framerateSecPerFrameIdx + 1) % GRVT_ARRAY_LENGTH(framerateSecPerFrame);
}


float FrameStatistics::Framerate() const {
	return (framerateSecPerFrameAccum > 0.0f) ? (1.0f / (framerateSecPerFrameAccum / (float)GRVT_ARRAY_LENGTH(framerateSecPerFrame))) : FLT_MAX;
}


float FrameStatistics::TimePerFrame() const {
	return 1000.0f / Framerate();
}