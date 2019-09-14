#include "stdafx.h"


void FloatingPinArray::Alloc(const int NewWidth, const int NewDepth) {
	_ASSERTE(NewWidth >= 0 && NewDepth >= 0);
	Width = NewWidth;
	Depth = NewDepth;

	Block.Reserve(Width * Depth);
}


void FloatingPinArray::BuildSurface(const int Surface, const int Tile) {
	typedef FloatingPin* (FloatingPinArray::*FindFunc)(FloatingPin &Pin, PinColumn &Column) const;
	static FindFunc Find[2] = {&FloatingPinArray::FindUpperOverlap, &FloatingPinArray::FindLowerOverlap};

	PinColumn &Col   = Block[Tile];
	PinColumn &ColE  = Block[Tile + 1];
	PinColumn &ColS  = Block[Tile + Width];
	PinColumn &ColSE = Block[Tile + Width + 1];

	for (FloatingPin &Pin : Col) {
		FloatingPin *PinE  = (this->*Find[Surface])(Pin, ColE);
		FloatingPin *PinS  = (this->*Find[Surface])(Pin, ColS);
		FloatingPin *PinSE = (this->*Find[Surface])(Pin, ColSE);

		if (PinE) {
			if (PinSE) {
				if (PinS) {
					// All 4 overlap
					float dltNWSE = fabs(Pin.Pos[Surface] - PinSE->Pos[Surface]);
					float dltNESW = fabs(PinE->Pos[Surface] - PinS->Pos[Surface]);

					if (dltNWSE < dltNESW) {
						Model.indices.Push(Pin.Vert[Surface]);
						Model.indices.Push(PinSE->Vert[Surface]);
						Model.indices.Push(PinE->Vert[Surface]);

						Model.indices.Push(Pin.Vert[Surface]);
						Model.indices.Push(PinS->Vert[Surface]);
						Model.indices.Push(PinSE->Vert[Surface]);
					} else {
						Model.indices.Push(Pin.Vert[Surface]);
						Model.indices.Push(PinS->Vert[Surface]);
						Model.indices.Push(PinE->Vert[Surface]);

						Model.indices.Push(PinS->Vert[Surface]);
						Model.indices.Push(PinSE->Vert[Surface]);
						Model.indices.Push(PinE->Vert[Surface]);
					}
				} else {
					// E/SE overlap
					Model.indices.Push(Pin.Vert[Surface]);
					Model.indices.Push(PinSE->Vert[Surface]);
					Model.indices.Push(PinE->Vert[Surface]);
				}
			} else if (PinS) {
				// E/S overlap
				Model.indices.Push(Pin.Vert[Surface]);
				Model.indices.Push(PinS->Vert[Surface]);
				Model.indices.Push(PinE->Vert[Surface]);
			}
		} else if (PinS && PinSE) {
			// S/SE overlap
			Model.indices.Push(Pin.Vert[Surface]);
			Model.indices.Push(PinS->Vert[Surface]);
			Model.indices.Push(PinSE->Vert[Surface]);
		}
	}
}

void FloatingPinArray::Build(const float Spacing) {
	Model.positions.Empty();
	Model.indices.Empty();

	glm::vec3 pos(0, 0, 0);
	for (int d = 0; d < Depth; d++) {
		pos.z = d * Spacing;

		for (int w = 0; w < Width; w++) {
			pos.x = w * Spacing;

			const int tile = d * Width + w;
			PinColumn &Col = Block[tile];

			for (FloatingPin &Pin : Col) {
				pos.y = Pin.Upper;
				Pin.UpperVert = (int)Model.positions.Length();
				Model.positions.Push(pos);

				Lines.positions.Push(pos);
				
				pos.y = Pin.Lower;
				Pin.LowerVert = (int)Model.positions.Length();
				Model.positions.Push(pos);

				Lines.positions.Push(pos);
			}
		}
	}

	for (int d = 0; d < Depth - 1; d++) {
		for (int w = 0; w < Width - 1; w++) {
			const int tile = d * Width + w;
			BuildSurface(0, tile);
			BuildSurface(1, tile);
		}
	}

	//Middleware::PackageMeshForBuild(&Model);
}


FloatingPin* FloatingPinArray::FindUpperOverlap(FloatingPin &Pin, PinColumn &Column) const {
	for (int p = (int)Column.Length() - 1; p >= 0; p--) {
		FloatingPin &PinB = Column[p];

		if (PinB.Lower >= Pin.Lower && PinB.Lower <= Pin.Upper ||
			PinB.Upper <= Pin.Upper && PinB.Upper >= Pin.Lower)
			return &PinB;
	}

	return nullptr;
}


FloatingPin* FloatingPinArray::FindLowerOverlap(FloatingPin &Pin, PinColumn &Column) const {
	for (int p = 0; p < Column.Length(); p++) {
		FloatingPin &PinB = Column[p];

		if (PinB.Upper <= Pin.Upper && PinB.Upper >= Pin.Lower ||
			PinB.Lower >= Pin.Lower && PinB.Lower <= Pin.Upper)
			return &PinB;
	}

	return nullptr;
}