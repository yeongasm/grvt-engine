#include "GrvtPch.h"
#include "Include/Core/ModelDefaults.h"


namespace Grvt
{

	//ModelCreationInfo GenerateDefaultCircle(uint32 EdgeSegments, uint32 RingSegments)
	//{
	//	ModelCreationInfo Info;

	//	for (uint32 y = 0; y <= RingSegments; y++)
	//	{
	//		for (uint32 x = 0; x <= EdgeSegments; x++)
	//		{
	//			float32 xSegment = (float32)x / (float32)EdgeSegments;
	//			float32 RingDepth = (float32)y / (float32)RingSegments;
	//			float32 PositionX = glm::cos(xSegment * glm::two_pi<float>());
	//			float32 PositionY = glm::sin(xSegment * glm::two_pi<float>());

	//			Info.PositionData.Push(glm::vec3(PositionX * RingDepth, PositionY * RingDepth, 0.0f));
	//		}
	//	}

	//	bool Odd = false;
	//	for (uint32 y = 0; y < RingSegments; y++)
	//	{
	//		if (!Odd)
	//		{
	//			for (uint32 x = 0; x <= EdgeSegments; x++)
	//			{
	//				Info.IndexData.Push( y		* (EdgeSegments + 1) + x);
	//				Info.IndexData.Push((y + 1) * (EdgeSegments + 1) + x);
	//			}
	//		}
	//		else
	//		{
	//			for (uint32 x = EdgeSegments; x >= 0; --x)
	//			{
	//				Info.IndexData.Push((y + 1) * (EdgeSegments + 1) + x);
	//				Info.IndexData.Push(y		* (EdgeSegments + 1) + x);
	//			}
	//		}

	//		Odd = !Odd;
	//	}

	//	Info.NormalsData.Reserve(Info.PositionData.Length());

	//	for (size_t i = 0; i < Info.IndexData.Length(); i += 3)
	//	{
	//		size_t Index0 = Info.IndexData[i];
	//		size_t Index1 = Info.IndexData[i + 1];
	//		size_t Index2 = Info.IndexData[i + 2];

	//		glm::vec3 Pos0 = Info.PositionData[Index0];
	//		glm::vec3 Pos1 = Info.PositionData[Index1];
	//		glm::vec3 Pos2 = Info.PositionData[Index2];

	//		glm::vec3 Normal0 = glm::normalize(glm::cross((Pos2 - Pos0), (Pos1 - Pos0)));
	//		glm::vec3 Normal1 = glm::normalize(glm::cross((Pos0 - Pos1), (Pos2 - Pos1)));
	//		glm::vec3 Normal2 = glm::normalize(glm::cross((Pos1 - Pos2), (Pos0 - Pos2)));

	//		Info.NormalsData[Index0] = glm::normalize(Info.NormalsData[Index0] + Normal0);
	//		Info.NormalsData[Index1] = glm::normalize(Info.NormalsData[Index1] + Normal1);
	//		Info.NormalsData[Index2] = glm::normalize(Info.NormalsData[Index2] + Normal2);
	//	}

	//	Info.TangentData.Reserve(Info.NormalsData.Length());
	//	Info.BitangentData.Reserve(Info.NormalsData.Length());
	//}


	ModelCreationInfo GenerateDefaultSphere(uint32 HorizontalSegments, uint32 VerticalSegments)
	{
		ModelCreationInfo Info;

		for (uint32 y = 0; y < VerticalSegments; y++)
		{
			for (uint32 x = 0; x < HorizontalSegments; x++)
			{
				float32 xSegment = (float32)x / (float32)VerticalSegments;
				float32 ySegment = (float32)y / (float32)HorizontalSegments;
				float32 xPos = glm::cos(xSegment * glm::two_pi<float32>()) * glm::sin(ySegment * glm::pi<float32>());
				float32 yPos = glm::cos(ySegment * glm::pi<float32>());
				float32 zPos = glm::sin(xSegment * glm::two_pi<float32>()) * glm::sin(ySegment * glm::pi<float32>());

				Info.PositionData.Push(glm::vec3(xPos, yPos, zPos));
				Info.TexCoordData.Push(glm::vec2(xSegment, ySegment));
				Info.NormalsData.Push(glm::vec3(xPos, yPos, zPos));
			}
		}

		for (uint32 y = 0; y < VerticalSegments; y++)
		{
			for (uint32 x = 0; x < HorizontalSegments; x++)
			{
				Info.IndexData.Push((y + 1) * (HorizontalSegments + 1) + x);
				Info.IndexData.Push( y		* (HorizontalSegments + 1) + x);
				Info.IndexData.Push( y		* (HorizontalSegments + 1) + x + 1);

				Info.IndexData.Push((y + 1) * (HorizontalSegments + 1) + x);
				Info.IndexData.Push( y		* (HorizontalSegments + 1) + x + 1);
				Info.IndexData.Push((y + 1) * (HorizontalSegments + 1) + x + 1);
			}
		}

		CalculateTangentAndBitangent(Info);

		return Info;
	}

}