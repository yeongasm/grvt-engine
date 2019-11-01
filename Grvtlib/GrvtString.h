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

	template <typename Type, size_t SlackMultiplier = 1>
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


		bool IsSmallString() const
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


		template <typename... ForwardType>
		void StoreInBuffer(size_t Index, ForwardType&&... Char)
		{
			Type* Ptr = PointerToString();
			Ptr[Index] = Type(Forward<ForwardType>(Char)...);
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

			Data = TempPtr;
			
			return true;
		}


		size_t WriteFv(const char* Literal)
		{
			Len = 0;
			size_t Length = strlen(Literal);

			if (Length >= Capacity) 
			{
				Alloc(Length + (1 << SlackMultiplier), false);
			}

			while (Len != Length) 
			{
				StoreInBuffer(Len++, Literal[Len]);
			}

			Type* Ptr = PointerToString();
			Ptr[Len] = '\0';

			return Len;
		}



		size_t WriteFv(const char* Format, va_list Arguments)
		{
			size_t Length = -1;

			// No one should ever need more than 256 Bytes of memory to write a formatted string.
			// NOTE(Afiq):
			// The size of this temporary buffer will change over time.
			char Buf[256];

			Length = vsnprintf(Buf, 256, Format, Arguments);

			// Encoding error!
			_ASSERTE(Length >= 0);

			return WriteFv(Buf);
		}


	public:

		BasicString() :
			Buffer{'\0'}, Capacity(SSO_MAX), Len(0) {}

		~BasicString()
		{
			Release();
		}


		BasicString(size_t Size) : BasicString()
		{
			Alloc(Size, false);
		}


		BasicString(const BasicString& Rhs)
		{
			*this = Rhs;
		}


		BasicString& operator= (const BasicString& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
			{
				Type* Ptr = Buffer;
				Type* RhsPtr = Rhs.PointerToString();

				// Perform a deep copy if the copied string is not a small string.
				if (!Rhs.IsSmallString())
				{
					Ptr = Data;
					Alloc(Rhs.Capacity, false);
				}

				Capacity = Rhs.Capacity;
				Len		 = Rhs.Len;

				//memcpy(Ptr, RhsPtr, Rhs.Capacity);
				for (size_t i = 0; i < Len; i++)
				{
					Ptr[i] = RhsPtr[i];
				}
			}

			return *this;
		}


		BasicString(BasicString&& Rhs)
		{
			*this = Move(Rhs);
		}


		BasicString& operator= (BasicString&& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != Rhs)
			{
				Type* Ptr = Buffer;
				Type* RhsPtr = Rhs.PointerToString();

				// Perform a deep copy if the copied string is not a small string.
				if (!Rhs.IsSmallString()) 
				{
					Ptr = Data;
				}

				Capacity = Rhs.Capacity;
				Len		 = Rhs.Len;

				for (size_t i = 0; i < Len; i++) 
				{
					Ptr[i] = RhsPtr[i];
				}

				new (&Rhs) BasicString();
			}

			return *this;
		}


		Type& operator[] (size_t Index)
		{
			if (Index >= Capacity)
				Alloc(Index + (1 << SlackMultiplier), true);

			Type* Ptr = PointerToString();
			return Ptr[Index];
		}


		const Type& operator[] (size_t Index) const
		{
			_ASSERTE(Index < Len);
			Type* Ptr = PointerToString();
			return Ptr[Index];
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
			return WriteFv(Literal);
		}

		void Empty()
		{
			Destruct(0, Len, true);
		}

		size_t Push(const Type& Char)
		{
			StoreInBuffer(Len++, Char);
			Type* Ptr = PointerToString();
			Ptr[Len] = '\0';

			return Len++;
		}

		size_t Push(Type&& Char)
		{
			StoreInBuffer(Len++, Move(Char));
			Type* Ptr = PointerToString();
			Ptr[Len] = '\0';

			return Len++;
		}

		const Type* C_Str() const
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