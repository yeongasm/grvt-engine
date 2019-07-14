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

using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint		= unsigned int;
using uint64	= unsigned __int64;


/**
* Iterator data structure.
*/
template <class ElementType>
class Iterator {
private:
	ElementType *pointer;

public:
	Iterator() : pointer(nullptr) {};

	Iterator(ElementType *Other) : pointer(Other) {}
	
	Iterator(const Iterator &Other) { *this = Other; }

	Iterator(Iterator &&Other) { *this = std::move(Other); }

	Iterator& operator= (const Iterator &Other) {
		_ASSERTE(this != &Other);

		if (this != &Other)
			pointer = Other.pointer;
		
		return *this;
	}

	Iterator& operator= (Iterator &&Other) {
		_ASSERTE(this != &Other);

		if (this != &Other) {
			pointer = Other.pointer;

			new (this) Iterator();
		}

		return *this;
	}

	Iterator& operator++ () {
		++pointer;
		return *this;
	}

	Iterator& operator-- () {
		--pointer;
		return *this;
	}

	bool operator!= (const Iterator& Other) const {
		return pointer != Other.pointer;
	}

	ElementType& operator* () const {
		return *pointer;
	}

	ElementType& operator+ (const size_t &Offset) {
		return *(pointer + Offset);
	}

	ElementType& operator- (const size_t &Offset) {
		return *(pointer - Offset);
	}

	ElementType& operator[] (const size_t & Offset) {
		return *(pointer + Offset);
	}
};


/**
* Templated hybrid Array. Can be used as a static Array or dynamic Array.
*
* This array's size grows exponentially!
* Internal capacity scales with the powers of two starting with 2 ^ 5 by default.
*/
template <class ElementType, unsigned int BufferBits = 5>
class Array {
public:

	using Iterator = Iterator<ElementType>;

protected:

	size_t	len;
	size_t	curPos;
	ElementType	*pArr;

private:

	void _Destruct(size_t FromIdx = 0, size_t ToIdx = curPos, const bool Reconstruct = false) {

		for (size_t i = FromIdx; i < ToIdx; i++) {
			pArr[i].~ElementType();

			if (Reconstruct)
				new (pArr + i) ElementType();
		}
	}

	/*
	* Set the length of the Array. Meant for the use of static Arrays.
	* When using the Array dynamically, call Push() instead as the Array knows how to manage it's own space.
	*/
	bool _Alloc(size_t Length = 0, bool Append = false) {
		ElementType *tempArr = nullptr;

		if (Length) {
			// If the function was initiated with a length.
			if (Length < len)
				_Destruct(Length, len);

			len = (Append) ? len + Length : Length;

			tempArr = (ElementType*)realloc(pArr, sizeof(ElementType) * len);

			if (!tempArr)
				return false;

			for (size_t i = curPos; i < len; i++)
				new (tempArr + i) ElementType();

			//curPos = len;
		} else if (!curPos || curPos == len) {

			len = (!curPos) ? 1 << BufferBits : len + (1 << BufferBits);

			tempArr = (ElementType*)realloc(pArr, sizeof(ElementType) * len);

			if (!tempArr)
				return false;

			for (size_t i = curPos; i < len; i++)
				new (tempArr + i) ElementType();
		}

		pArr = tempArr;

		return true;
	}

	/**
	* We now use perfect forwarding to distinguish if the pushed in element a lvalue or rvalue.
	* This solves the problem of custom data types deleting copy assignment operator or move assignment operator.
	*/
	template <class... _Type>
	void _StoreIntoBuffer(_Type &&...Element) {
		if (curPos == len) {
			_Alloc();
		}

		pArr[curPos] = ElementType(std::forward<_Type>(Element)...);
	}

public:

	/**
	* Default constructor used to initialise the empty Array.
	*/
	Array() : pArr(nullptr), curPos(0), len(0) {}

	/**
	* Copy constructor used to copy elements from another Array.
	*/
	Array(const Array &Other) : Array() {
		*this = Other;
	}

	/**
	* Move semantics.
	*/
	Array(Array &&Other) {
		_ASSERTE(this != &Other);

		*this = std::move(Other);

		//pArr = Other.pArr;
		//len = Other.len;
		//curPos = Other.curPos;

		//new (&Other) Array();
	}

	/**
	* Constructor to initialise the Array with an initialiser list.
	*/
	Array(const std::initializer_list<ElementType> &InitList) : Array() {
		if (InitList.size() > len) {
			if (!_Alloc((size_t)InitList.size())) {
				// If upsizing the Array fails, throw an assertion.
				_ASSERTE(false);
			}
		}

		curPos = (size_t)InitList.size();

		memcpy(pArr, InitList.begin(), sizeof(ElementType) * InitList.size());
	}

	/*
	* Constructor to initialise a 'static' Array of sorts.
	* The ability to Push() new elements into the Array is still do-able.
	*/
	Array(const size_t &Length) : pArr(nullptr), curPos(0) {
		_Alloc(Length);
	}

	/**
	* Operator to copy the contents from another Array into this one.
	*/
	Array& operator= (const Array &Other) {
		if (this != &Other) {
			_Alloc(Other.Size());

			curPos = Other.curPos;

			//memcpy(pArr, Other.pArr, Other.cusPos - 1);
			for (size_t i = 0; i < Other.Length(); i++) {
				pArr[i] = Other.pArr[i];
			}
		}

		return *this;
	}

	/**
	* Operator for move semantics.
	*/
	Array& operator= (Array &&Other) {
		_ASSERTE(this != &Other);

		Release();

		pArr = Other.pArr;
		len = Other.len;
		curPos = Other.curPos;

		new (&Other) Array();

		return *this;
	}

	ElementType& operator[] (const size_t Idx) {
		// The index specified exceeded the internal buffer size of the Array.
		_ASSERTE(Idx < len);

		// In a scenario where the [] operator is used to assign values to the Array, the Array needs to increase it's internal capacity when needed.
		if (Idx == len)
			_Alloc();

		// Keeps track of the last element index inside the Array.
		if (curPos <= Idx)
			curPos = Idx + 1;

		return pArr[Idx];
	}

	const ElementType& operator[] (const size_t Idx) const {
		// The index specified exceeded the internal buffer size of the Array.
		_ASSERTE(Idx < len);

		return pArr[Idx];
	}

	~Array() {
		Release();
	}

	/**
	* Allocate the specified amount of space for the Array increasing the internal capacity.
	*
	* @param Length - The desired newly allocated internal buffer size.
	* @param Append - Appends to specified allocated size to the current internal capacity.
	*/
	bool Reserve(size_t Length, bool Append = true) {
		return _Alloc(Length, Append);
	}

	/**
	* Free all the elements in the Array, releasing it's contents.
	*/
	void Release() {
		_Destruct(0, len);

		free(pArr);

		new (this) Array();
	}

	/**
	* Push an element into the the back of the Array.
	* If elements were assigned via the static method and there are empty slots within the Array, the Array class would
	* ignore the index of the gap and push new elements to the back of the Array.
	* First-In-Last-Out implementation.
	* Returns the index of the newly allocated Element within the Array.
	*
	* @param Element - An element to be pushed into the back of the Array.
	*/
	size_t Push(const ElementType &Element) {
		_StoreIntoBuffer(Element);

		return curPos++;
	}
	
	/**
	* Push an element into the the back of the Array.
	* If elements were assigned via the static method and there are empty slots within the Array, the Array class would
	* ignore the index of the gap and push new elements to the back of the Array.
	* First-In-Last-Out implementation.
	* Returns the index of the newly allocated Element within the Array.
	*
	* @param Element - An element to be pushed into the back of the Array.
	*/
	size_t Push(ElementType &&Element) {
		_StoreIntoBuffer(std::move(Element));

		return  curPos++;
	}

	/**
	* Push an element into the the back of the Array.
	* If elements were assigned via the static method and there are empty slots within the Array, the Array class would
	* ignore the index of the gap and push new elements to the back of the Array.
	* First-In-Last-Out implementation.
	* Returns a reference / pointer to the newly allocated Element within the Array.
	*
	* @param Element - An element to be pushed into the back of the Array.
	*/
	template <class _Type>
	decltype(auto) Insert(_Type &&Element) {
		size_t idx = Push(std::forward<_Type>(Element));

		return pArr[idx];
	}

	/**
	* Appends an existing Array and copy or move it's elements into the back of the Array.
	* Destroys contents from the copied Array and constructs a new one by default.
	* Returns the index of the last element after appending.
	*
	* @param Other   - The foreign Array that is being appended.
	* @param Release - Release the foreign Array's memory. [Default] true.
	*/
	size_t Append(const Array &Other, bool Release = true) {
		MoveOrCopy(Other, false, Release);

		return curPos;
	}

	/**
	* Appends new elements into the Array with an initializer list.
	* Returns the index of the last element after appending.
	*
	* @param InitList - A foreign initializer list.
	*/
	size_t Append(const std::initializer_list<ElementType> &InitList) {
		for (size_t i = 0; i < InitList.size(); i++)
			Push(*(InitList.begin() + i));

		return curPos - 1;
	}

	/**
	* Append mutiple elements into the Array by supplying a beginning pointer and the total elements from the foreign container.
	* If the amount inserted exceeds the the internal buffer of the Array, the Array memory reallocation is done.
	* Does not destroy the foreign container's contents.
	* Returns the index of the last element within the Array after appending.
	*
	* @param Begin	- Foreign container's beginning pointer.
	* @param Length	- Foreign container's end pointer.
	*/
	size_t Append(Iterator Begin, const size_t &Length) {
		for (size_t i = 0; i < Length; i++)
			Push(Begin + i);

		return curPos - 1;
	}

	/**
	* Moves all the element from the other Array into the existing one or simply copies from it.
	*
	* @param Other      - The foreign Array that is being moved/copied.
	* @param RemLocal   - Clears all the existing elements in the local Array. [Default] true.
	* @param RemForeign - Clears all the existing element in the foreign Array. [Default] true.
	*/
	void MoveOrCopy(Array &Other, bool RemLocal = true, bool RemForeign = true) {

		if (RemLocal)
			Release();

		for (size_t i = 0; i < Other.Length(); i++)
			Push(Other[i]);

		if (RemForeign)
			Other.Release();
	}

	/**
	* Pop an element from the back of the Array.
	* Last-In-First-Out implementation.
	* Returns the last index in the element.
	*
	* @param Count	  - The amount of element(s) to be removed. [Default] 1.
	* @param Shrink	  - Reduce the size of the internal buffer allocator for the Array. [Default] false.
	*/
	size_t Pop(size_t Count = 1, bool Shrink = false) {
		_ASSERTE(curPos >= 0);

		_Destruct(curPos - Count, curPos, true);

		curPos -= Count;

		if (curPos < 0)
			curPos = 0;

		if (Shrink)
			ShrinkToFit();

		return curPos;
	}

	/**
	* Pops an element at the specified index.
	* Optionally moves the contents of the Array. Optionally shrinks the length of the Array.
	* Reconstructs a fresh state in that index by calling the object's constructor.
	*
	* @param Idx	- The variable's index within the container.
	* @param Move	- Moves the contents within the container closing any gaps in indices. [Default] true.
	* @param Shrink	- Reduces the internal buffer size of the container. [Default] false.
	*/
	void PopAt(const size_t Idx, bool Move = true, bool Shrink = false) {

		// The element should reside in the container.
		_ASSERTE(Idx < len);

		pArr[Idx].~ElementType();

		if (Move) {
			//memmove(pArr + Idx, pArr + Idx + 1, sizeof(Type) * --curPos - Idx);
			for (size_t i = Idx; i < Length() - 1; i++) {
				pArr[i] = pArr[i + 1];
			}

			new (pArr + --curPos) ElementType();

			//curPos--;
		} else {
			new (pArr + Idx) ElementType();
		}

		if (Shrink)
			ShrinkToFit();
	}

	/**
	* Pops all elements out of the container.
	* Unlike Release(), Empty() maintains the internal capacity of the Array.
	* Pushing a new element would insert it back at index 0.
	* Optionally, reconstruct the element to it's default state.
	*
	* @param Reconstruct - Reconstruct all elements inside the container to it's default state. [Default] true.
	*/
	void Empty(bool Reconstruct = true) {
		_Destruct(0, curPos, Reconstruct);

		curPos = 0;
	}

	/**
	* Shrinks the size of the Array to perfectly fit it's contents.
	* This means storing only enough space to hold the total elements inside the Array.
	* Returns true on success, false on error.
	*/
	bool ShrinkToFit() {
		ElementType *tempArr = nullptr;

		_Destruct(curPos, len);
		size_t length = (curPos <= len ) ? curPos : len;

		if (length < len) {
			tempArr = (ElementType*)realloc(pArr, sizeof(ElementType) * len);
			
			if (!tempArr)
				return false;

			pArr = tempArr;
		}

		return true;
	}

	/**
	* Returns the index of the specified object in the Array by reference.
	* Not to be used for Arrays storing pointers. Check Find() function.
	*
	* @param Element - An element passed in as value.
	*/
	inline size_t IndexOf(const ElementType &Element) const {
		return IndexOf(&Element);
	}

	/**
	* Returns the index of the specified object in the Array.
	* Not to be used for Arrays storing pointers. Check Find() function.
	*
	* @param Element - An element passed in by pointer.
	*/
	inline size_t IndexOf(const ElementType *Element) const {
		// Object must reside within the boundaries of the Array.
		_ASSERTE(Element >= pArr && Element < pArr + len);

		// The specified object should reside at an address that is an even multiple of of the Type's size.
		_ASSERTE(((unsigned __int8*)Element - (unsigned __int8*)pArr) % sizeof(ElementType) == 0);

		return (size_t)(Element - pArr);
	}

	/**
	* Retrieves the index of the element in the Array.
	* Returns a -1 if not found.
	* If there are multiple similar elements
	*
	* @param Element - Object to be evaluated.
	*/
	inline size_t Find(const ElementType &Element) const {
		for (size_t i = 0; i < curPos; i++)
			if (Element == pArr[i])
				return i;

		return -1;
	}

	/**
	* Iterates the Array and checks if the specified element exists.
	* Returns true if exist, false if not.
	*
	* @param Element - Object to be evaluated.
	*/
	inline bool Has(const ElementType &Element) const {
		return Find(Element) != -1;
	}

	/**
	* Returns the index of the end-most object within the Array.
	*/
	inline size_t Length() const {
		return curPos;
	}

	/**
	* Returns the internal buffer size allocated inside the Array.
	*/
	inline size_t Size() const {
		return len;
	}

	/**
	* Returns a pointer to the first element in the Array.
	*/
	inline ElementType* First() {
		return pArr;
	}

	/**
	* Returns a reference to the first element in the Array.
	*/
	inline ElementType& Front() {
		return pArr[0];
	}

	/**
	* Returns a pointer to the last element in the Array.
	* If length of array is 0, Last() returns the 0th element.
	*/
	inline ElementType& Last() {
		size_t idx = 0;

		idx = curPos - 1;

		if (!curPos)
			idx = 0;

		return pArr[idx];
	}

	/**
	* Returns a reference to the first element in the Array.
	* If the length of the Array is 0, Back() returns the 0th element.
	*/
	//inline ElementType& Back() {
	//	return *Last();
	//}

	/**
	* Range-based loop initialiser.
	*/
	Iterator begin() const {
		return Iterator(pArr);
	}

	/**
	* Range-based loop terminator.
	*/
	Iterator end() const {
		return Iterator(pArr + curPos);
	}

	/**
	* Reverse iterator begin.
	*/
	Iterator rbegin() const {
		return Iterator(pArr + curPos);
	}

	/**
	* Reverse iterator end.
	*/
	Iterator rend() const {
		return Iterator(pArr);
	}
};


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
			Release();

		Array::Reserve(Len + 1, false);

		//memcpy(pArr, Buffer, sizeof(char) * Len);
		for (size_t i = 0; i < Len + 1; i++)
			pArr[i] = Buffer[i];
		// The index of the null terminated character should be at a position that is the length of the string. REMINDER: Arrays start with 0!
		pArr[Len] = '\0';
		curPos = Len + 1;

		UpdateHash();

		// Shrink the buffer to perfectly fit it's contents.
		ShrinkToFit();
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


/**
* Templated Pair Data Structure.
* Used for storing data in pairs. In no way that Pair would replace std::tuple.
*/
template <class KeyType, class ValueType>
class Pair {
public:

	KeyType		Key;
	ValueType	Value;

	Pair() : Key(KeyType()), Value(ValueType()) {};

	Pair(const KeyType &OtherKey, const ValueType &OtherValue) :
		Key(OtherKey), Value(OtherValue) {};

	Pair(KeyType &&OtherKey, ValueType &&OtherValue) :
		Key(OtherKey), Value(OtherValue) {};

	Pair(const std::initializer_list<Pair<KeyType, ValueType>> &InitList) :
		Key(InitList[0]), Value(InitList[1]) {};

	Pair(const Pair &Other) {
		*this = Other;
	}

	Pair(Pair &&Other) {
		*this = std::move(Other);
	}

	Pair& operator= (const Pair &Other) {
		Key = Other.Key;
		Value = Other.Value;

		return *this;
	}

	Pair& operator= (Pair &&Other) {
		_ASSERTE(this != &Other);

		Key = Other.Key;
		Value = Other.Value;

		new (&Other) Pair();

		return *this;
	}

	~Pair() {
		Release();
	}

	void Release() {
		Key.~KeyType();
		Value.~ValueType();
	}
};

#endif // !GRAVITY_STL