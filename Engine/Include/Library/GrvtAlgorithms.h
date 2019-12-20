#pragma once

#ifndef GRAVITY_LIB_ALGORITHMS
#define GRAVITY_LIB_ALGORITHMS

#include "Library/GrvtArray.h"

namespace Gfl
{

	template <typename T>
	void Swap(T* First, T* Second)
	{
		T Temp = *First;
		*First = *Second;
		*Second = Temp;
	}


	template <typename T>
	void Swap(T& First, T& Second)
	{
		Swap<T>(&First, &Second);
	}


	namespace Ga
	{
		template<typename T>
		uint32 _Paritition(T* Data, uint32 Low, uint32 High)
		{
			T Pivot = Data[High];
			uint32 Index = Low - 1;

			for (uint32 i = Low; i <= High; i++)
			{
				if (Data[i] < Pivot)
				{
					Index++;
					Swap(Data[Index], Data[i]);
				}
			}
			
			Swap(Data[Index + 1], Data[High]);

			return Index + 1;
		}

		/**
		* TODO(Afiq):
		* Understand the algorithm and improvise the function.
		*/
		template <typename T>
		void _QuickSort(T* Data, uint32 Low, uint32 High)
		{
			if (Low > High)
			{
				return;
			}

			uint32 Pivot = _Paritition(Data, Low, High);

			_QuickSort(Data, Low, Pivot);
			_QuickSort(Data, Pivot + 1, High);
		}
	}


	/**
	* QuickSort algorithm.
	*
	* Element type stored in the array must overload '<' operator if types are non-native.
	* Containers used should provide both of these functions:
	*
	* 1. First() - Returns a pointer to the first element in the container.
	* 2. Length() - The total number of elements in the container.
	*/
	template <typename ContainerType>
	void QuickSort(ContainerType& Source)
	{
		Ga::_QuickSort(Source.First(), 0, Source.Length());
	}

	
}


#endif // !GRAVITY_LIB_ALGORITHMS