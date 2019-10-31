#pragma once
#ifndef GRAVITY_LIB_STRING
#define GRAVITY_LIB_STRING


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "GrvtFoundation.h"
#include "GrvtIterator.h"


namespace Gfl
{

#define BITS_IN_A_BYTE 8

	template <typename Type, size_t SlackMultiplier = 5>
	class BasicString
	{
	public:

		using Iterator = Iterator<Type>;

	private:
		

		static constexpr size_t SSO_MAX = 16 / sizeof(Type) < 1 ? 1 : 16 / sizeof(Type);

		union
		{
			Type* Data;
			Type  Buffer[SSO_MAX];
		};

		size_t Capacity;
		size_t Len;


		bool IsSmallString()
		{
			return Capacity <= SSO_MAX;
		}


		Type* PointerToString()
		{
			Type* Ptr = Buffer;
			if (!IsSmallString())
				Ptr = Data;

			return Ptr;
		}


		const Type* PointerToString() const
		{
			const Type* Ptr = Buffer;
			if (!IsSmallString())
				Ptr = Data;

			return Ptr;
		}


		/**
		* Stores the string literal into the buffer.
		*
		* @param Literal - The string literal that is to be stored.
		* @param Length - Length of the new string excluding the null terminator.
		*/
		void StoreInBuffer(const char* Literal, size_t Length)
		{
			Type* Ptr = IsSmallString() ? Buffer : Data;
			memcpy(Ptr, Literal, Length + 1);
			Ptr[Length] = '\0';
		}


		void Destruct(size_t From = 0, size_t To = Len, bool Reconstruct = false)
		{
			Type* Ptr = PointerToString();
			for (size_t i = From; i < To; i++)
			{
				Ptr[i].~Type();
				if (Reconstruct)
					new (Ptr + i) Type();
			}
		}


		bool Alloc(size_t Length, bool Append)
		{
			Type* TempPtr = nullptr;

			if (Length < Capacity)
				Destruct(Length, Capacity);

			Capacity = Append ? Capacity + Length : Length;
			TempPtr = (Type*)realloc(Data, Capacity);
			
			if (!TempPtr)
				return false;

			for (size_t i = Len; i < Capacity; i++)
			{
				new (TempPtr + i) Type();
			}

			Data = TempPtr;
			
			return true;
		}


		size_t WriteFv(const char* Format, va_list Arguments)
		{
			//size_t Length = -1;

			//Length = vsnprintf(nullptr, 0, Format, Arguments);

		}


	public:

		BasicString() :
			Buffer{'\0'}, Capacity(SSO_MAX), Len(0) {}

		~BasicString()
		{
			Release();
		}

		void Release()
		{
			Destruct(0, Capacity);
			if (!IsSmallString())
				free(Data);

			new (this) BasicString();
		}

		bool Reserve(size_t Length, bool Append = true)
		{
			if (Length <= SSO_MAX)
				return false;

			return Alloc(Length, Append);
		}

		size_t Format(const char* Format, ...)
		{
			va_list Args;
			va_start(Args, Format);
			size_t Length = WriteFv(Format, Args);
			va_end(Args);

			return Length;
		}

		size_t Write(const char* Literal)
		{
			size_t Length = strlen(Literal);
			
			if (Length > SSO_MAX)
			{
				Alloc(Length + (1 << SlackMultiplier), false);
			}
			
			StoreInBuffer(Literal, Length);
			Len = Length;
			return Len;
		}

		const char* C_Str() const
		{
			return PointerToString();
		}
		
		size_t Length() const
		{
			return Len;
		}

		size_t Size() const
		{
			return Capacity;
		}

		Type* First()
		{
			return PointerToString();
		}

		Type* Last()
		{
			return PointerToString() + Len;
		}

		Type& Front()
		{
			return *PointerToString();
		}

		Type& Back()
		{
			return *(PointerToString() + Len);
		}

	};


	using String  = BasicString<char>;
	using WString = BasicString<wchar_t>;

}


#endif GRAVITY_LIB_STRING