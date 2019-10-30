#pragma once
#ifndef GRAVITY_STL
#define GRAVITY_STL

#include <crtdbg.h>
#include <cstdlib>
#include <initializer_list>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <utility>
#include <intrin.h>

/**
* Simple and easy to use String data structure.
* Avoid constructing a new string by assigning to a constant string literal. This is due to the fact that C++ is unable to differentiate constructors that takes 
  a const char* and a const char* with variadic arguments. Hence assigning a new string object to a constant literal will call WriteFV() that allocates a temporary buffer on the heap.
* It is recommended to assign a constant literal to the string objet by assigning it inside of SetString();
* It is also recommended to compare strings with the object itself. The String object generates a hash that does faster string comparison.
*
* TODO(Afiq):
* Implement Boyer Moore Algorithm for string search function.
* IMPORTANT!!! The string class really needs to be redone. It must no longer be derived from the Array class.
*/
class String : public Array<char> {
private:
	uint Hash;

	/**
	* Pushes all the characters from the string literal into the Array buffer.
	* Existing buffer is released upon assinging a new string literal.
	*/
	void _PushStringIntoBuffer(const char *Buffer, const size_t &Len) {
		// Release the existing buffer from the Array.
		if (curPos > 0)
			Empty();

		Array::Reserve(Len + 1, false);

		//memcpy(pArr, Buffer, sizeof(char) * Len);
		for (size_t i = 0; i < Len + 1; i++)
			pArr[i] = Buffer[i];
		// The index of the null terminated character should be at a position that is the length of the string. REMINDER: Arrays start with 0!
		pArr[Len] = '\0';
		curPos = Len + 1;

		UpdateHash();

		// Shrink the buffer to perfectly fit it's contents.
		//ShrinkToFit();
	}

	/**
	* Sets a formatted string.
	*/
	size_t _WriteFV(const char *Format, va_list Args) {
		size_t len = -1;

		va_list args2;
		va_copy(args2, Args);
		len = vsnprintf(NULL, 0, Format, args2);

		// Encoding error! This should really be replaced with a log feature in the future.
		_ASSERTE(len >= 0);

		char* buff = nullptr;
		buff = (char*)malloc(sizeof(char) * (len + 1));
		vsnprintf(buff, len + 1, Format, args2);
		buff[len] = '\0';

		// Encoding error! This should really be replaced with a log feature in the future.
		_ASSERTE(len >= 0);

		// Push the string literal into our storage buffer and release our temporary buffer from memory.
		_PushStringIntoBuffer(buff, len);
		free(buff);

		return len;
	}

public:

	String() : Hash(0) {}

	String(const char *Format, ...) : Hash(0) {
		va_list args;
		va_start(args, Format);
		_WriteFV(Format, args);
		va_end(args);
	}

	String(const String &Other) {
		*this = Other;
	}

	String(String &&Other) {
		// Ensures that it is not moving it's own data.
		_ASSERTE(this != &Other);

		*this = std::move(Other);
	}

	size_t operator() (const String& Other) {
		return UpdateHash();
	}

	String& operator= (const String &Other) {
		// Ensures the other string is properly null terminated.
		//_ASSERTE(Other.pArr[Other.Length()] == '\0');
		if (Other.pArr)
			_PushStringIntoBuffer(Other.pArr, Other.Length());

		return *this;
	}

	String& operator= (String &&Other) noexcept {
		// Ensures that it is not assigning to it's own data.
		_ASSERTE(this != &Other);

		Release();

		pArr	= Other.pArr;
		curPos	= Other.curPos;
		len		= Other.len;
		Hash	= Other.Hash;

		new (&Other) String();

		return *this;
	}

	~String() {
		Release();
	}

	/**
	* Overloaded equality operator for a const String source.
	*/
	bool operator== (const String &Other) {
		return (Hash == Other.Hash) ? true : this->c_str() == Other.c_str() ;
	}

	/**
	* Overloaded inequality operator for a const String source.
	*/
	bool operator!= (const String &Other) {
		return !(*this == Other);
	}

	/**
	* Overloaded assignment operator.
	* Assigns a new string literal to the buffer.
	*/
	String& operator= (const char *String) {
		SetString(String);

		return *this;
	}

	/**
	* Overload Array's Release() function.
	* This sets the hash of the string back to 0 as well.
	*/
	void Release() {
		Array::Release();
		Hash = 0;
	}

	/**
	* Overloaded less than operator.
	* Enables usability as a key in std::map.
	*/
	bool operator< (const String &Other) const {
		return (Hash < Other.Hash);
	}

	/**
	* Overloaded ~ operator.
	* It is the same as calling c_str();
	*/
	inline char* operator~ () const {
		return pArr;
	}

	/**
	* Prints the content of the string from the buffer.
	* Returns true on sucess, false if buffer is empty.
	*/
	bool Print(bool NewLine = true) const {
		if (!pArr)
			return false;

		const char *param = (NewLine) ? "%s\n" : "%s";

		printf(param, c_str());

		return true;
	}

	/**
	* Sets the string.
	* Returns the length of a newly constructed string excluding the null terminator.
	* Returns -1 or error.
	*/
	size_t Write(const char *Format, ...) {
		va_list args;
		va_start(args, Format);
		size_t length = _WriteFV(Format, args);
		va_end(args);

		return length;
	}

	/**
	* Assign a new string literal into the buffer.
	* This function is equivalent to assigning the buffer with the asignment operator
	*
	* @param String - String to be placed into the buffer.
	*/
	void SetString(const char *String) {
		// Get the length of the string excluding the null terminated character.
		size_t length = (size_t)strlen(String);

		_PushStringIntoBuffer(String, length);
	}

	/**
	* Updates the string hash.
	* Returns the newly updated hash.
	*/
	size_t UpdateHash() {
		Hash = 0;

		// Currently we are using the sdbm hashing algorithm. Should change for future proofing.
		for (size_t i = 0; i < curPos - 1; i++)
				Hash = _mm_crc32_u8(Hash, pArr[i]);

		return Hash;
	}

	/**
	* Returns the pointer to the string literal inside the buffer.
	*/
	inline const char* c_str() const {
		return pArr;
	}

	/**
	* Returns the length of the string excluding it's null term
	*/
	inline size_t Length() const {
		if (!pArr)
			return 0;

		size_t len = (size_t)strlen(pArr);

		return len;
	}
};


#endif // !GRAVITY_STL