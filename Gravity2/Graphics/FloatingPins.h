#pragma once


struct FloatingPin {
	union {
		struct {
			float	Upper;
			float	Lower;
		};

		float		Pos[2];
	};

	union {
		struct {
			int		UpperVert;
			int		LowerVert;
		};

		int			Vert[2];
	};

	FloatingPin() = default;
	
	FloatingPin(const float Upper, const float Lower) :
		Upper(Upper), Lower(Lower) {}
};


typedef Array<FloatingPin>	PinColumn;
typedef Array<PinColumn>	PinBlock;

class FloatingPinArray {
protected:
	void BuildSurface(const int Surface, const int Tile);

public:
	PinBlock	Block;
	Mesh		Model;

	int			Width;
	int			Depth;

	Mesh		Lines;

	void Alloc(const int NewWidth, const int NewDepth);
	void Build(const float Spacing);

	FloatingPin* FindUpperOverlap(FloatingPin &Pin, PinColumn &Column) const;
	FloatingPin* FindLowerOverlap(FloatingPin &Pin, PinColumn &Column) const;
};
