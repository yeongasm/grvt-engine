#pragma once


/**
* Framerate data structure.
*
* Benchmark the time taken to render a single frame in milliseconds (ms).
* Benchmark the total amount of frames per second (FPS) the application is running at.
*/
class FrameStatistics {
private:
	int		framerateSecPerFrameIdx;
	float	framerateSecPerFrameAccum;
	float	framerateSecPerFrame[120];

public:

	FrameStatistics();

	FrameStatistics(const FrameStatistics &Other);
	FrameStatistics(FrameStatistics &&Other);

	FrameStatistics& operator= (const FrameStatistics &Other);
	FrameStatistics& operator= (FrameStatistics &&Other);

	~FrameStatistics();


	/**
	* Computes the values needed for frame statistics.
	* An instance of this function is automatically called inside of the application's Tick() function.
	*
	* @param [REQUIRED] (float) DeltaTime - Time between the current frame with the previous frame.
	*/
	void Tick(float DeltaTime);


	/**
	* Returns the framerate of the application.
	* Should not be used as a metric to measure performance as there can be delay between the CPU and the GPU.
	*/
	float Framerate() const;


	/**
	* Returns the time in milliseconds (ms) passed between one frame to the other.
	* Recommended to be used as a metric to measure performance.
	*/
	float TimePerFrame() const;
};