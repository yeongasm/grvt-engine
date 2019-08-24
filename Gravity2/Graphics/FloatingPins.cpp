#include "stdafx.h"


void FloatingPinArray::Alloc(const int NewWidth, const int NewDepth) {
	_ASSERTE(NewWidth >= 0 && NewDepth >= 0);
	Width = NewWidth;
	Depth = NewDepth;

	Block.Reserve(Width * Depth);
}


void FloatingPinArray::Build(const float Spacing) {
	Model.positions.Empty();
	Model.indices.Empty();

	glm::vec3 pos(0, 0, 0);
	for (int d = 0; d < Depth; d++) {
		pos.z = d * Spacing;

		for (int w = 0; w < Width; w++) {
			pos.x = w * Spacing;

			PinColumn &Col   = Block[d * Width + w];

			for (FloatingPin &Pin : Col) {
				pos.y = Pin.Upper;

				Pin.UpperVert = Model.positions.Length();
				Model.positions.Push(pos);
			}
		}
	}

	for (int d = 0; d < Depth - 1; d++) {
		for (int w = 0; w < Width - 1; w++) {
			PinColumn &Col   = Block[d * Width + w];
			PinColumn &ColE  = Block[d * Width + w + 1];
			PinColumn &ColS  = Block[d * Width + w + Width];
			PinColumn &ColSE = Block[d * Width + w + Width + 1];

			for (FloatingPin &Pin : Col) {
				FloatingPin *PinE  = FindUpperOverlap(Pin, ColE);
				FloatingPin *PinS  = FindUpperOverlap(Pin, ColS);
				FloatingPin *PinSE = FindUpperOverlap(Pin, ColSE);

				if (PinE) {
					if (PinSE) {
						if (PinS) {
							// All 4 overlap
							float dltNWSE = fabs(Pin.Upper - PinSE->Upper);
							float dltNESW = fabs(PinE->Upper - PinS->Upper);

							if (dltNWSE < dltNESW) {
								Model.indices.Push(Pin.UpperVert);
								Model.indices.Push(PinSE->UpperVert);
								Model.indices.Push(PinE->UpperVert);

								Model.indices.Push(Pin.UpperVert);
								Model.indices.Push(PinS->UpperVert);
								Model.indices.Push(PinSE->UpperVert);
							} else {
								Model.indices.Push(Pin.UpperVert);
								Model.indices.Push(PinS->UpperVert);
								Model.indices.Push(PinE->UpperVert);

								Model.indices.Push(PinS->UpperVert);
								Model.indices.Push(PinSE->UpperVert);
								Model.indices.Push(PinE->UpperVert);
							}
						} else {
							// E/SE overlap
							Model.indices.Push(Pin.UpperVert);
							Model.indices.Push(PinSE->UpperVert);
							Model.indices.Push(PinE->UpperVert);
						}
					} else if (PinS) {
						// E/S overlap
						Model.indices.Push(Pin.UpperVert);
						Model.indices.Push(PinS->UpperVert);
						Model.indices.Push(PinE->UpperVert);
					}
				} else if (PinS && PinSE) {
					// S/SE overlap
					Model.indices.Push(Pin.UpperVert);
					Model.indices.Push(PinS->UpperVert);
					Model.indices.Push(PinSE->UpperVert);
				}
			}
		}
	}

	//Middleware::PackageMeshForBuild(&Model);
}


FloatingPin* FloatingPinArray::FindUpperOverlap(FloatingPin &Pin, PinColumn &Column) {
	for (int p = Column.Length() - 1; p >= 0; p--) {
		FloatingPin &PinB = Column[p];

		if (PinB.Lower >= Pin.Lower && PinB.Lower <= Pin.Upper ||
			PinB.Upper <= Pin.Upper && PinB.Upper >= Pin.Lower)
			return &PinB;
	}

	return nullptr;
}


FloatingPin* FloatingPinArray::FindLowerOverlap(FloatingPin &Pin, PinColumn &Column) {
	for (int p = 0; p < Column.Length(); p++) {
		FloatingPin &PinB = Column[p];

		if (PinB.Upper <= Pin.Upper && PinB.Upper >= Pin.Lower)
			return &PinB;
	}

	return nullptr;
}