#pragma once


struct FloatingPin {
	float	Upper;
	float	Lower;

	int		UpperVert;
	int		LowerVert;

	FloatingPin() = default;
	
	FloatingPin(const float Upper, const float Lower) :
		Upper(Upper), Lower(Lower) {}
};


typedef Array<FloatingPin>	PinColumn;
typedef Array<PinColumn>	PinBlock;

class FloatingPinArray {
public:
	PinBlock	Block;
	Mesh		Model;

	int			Width;
	int			Depth;

	void Alloc(const int NewWidth, const int NewDepth);
	void Build(const float Spacing);

	FloatingPin* FindUpperOverlap(FloatingPin &Pin, PinColumn &Column);
	FloatingPin* FindLowerOverlap(FloatingPin &Pin, PinColumn &Column);
};