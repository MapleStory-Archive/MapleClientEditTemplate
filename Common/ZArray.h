#pragma once
#include "logger.h"
#include "ZAllocEx.h"

template <typename T>
struct ZArray
{
private:
	T* a;

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	ZArray()
	{
		this->a = nullptr;
	}

	/// <summary>
	/// Allocating constructor
	/// </summary>
	/// <param name="uSize">Number of spaces to allocate for in the array</param>
	ZArray(size_t uSize)
	{
		this->a = nullptr;
		this->Alloc(uSize);
	}

	~ZArray()
	{
		this->RemoveAll();
	}

	ZArray<T>* operator=(ZArray<T>* r)
	{
		// TODO
	}

	T* operator[](int i)
	{
		return &this->a[i];
	}

	BOOL IsEmpty()
	{
		return this->GetCount() == 0;
	}

	T* Insert()
	{
		return nullptr; // TODO
	}

	T* InsertBefore(int nIndex)
	{
		return nullptr; // TODO
	}

	void RemoveAt(T* pos)
	{
		// TODO
	}

	size_t GetCount()
	{
		return this->a ? reinterpret_cast<DWORD*>(this->a)[-1] : 0;
	}

	T* GetNext(T** pos)
	{
		T* result = *pos;

		*pos = *pos > this->a ? result - 1 : nullptr;

		return result;
	}

	T* GetPrev(T** pos)
	{
		T* result = *pos;

		/* Highest index is array size - 1 */
		DWORD nIndex = *(reinterpret_cast<DWORD*>(this->a) - 1); // get array size
		nIndex -= 1; // reduce index by 1

		if (*pos >= &this->a[nIndex]) // TODO verify this is correct
		{
			*pos = nullptr;
		}
		else
		{
			*pos = result + 1;
		}

		return result;
	}

	T* GetHeadPosition()
	{
		return nullptr; // TODO
	}

	void RemoveAll()
	{
		if (this->a)
		{
			DWORD* pAllocationBasePointer = reinterpret_cast<DWORD*>(this->a) - 1;

			/* Call destructor  */
			this->Destroy(this->a, &this->a[*pAllocationBasePointer]);

			/* Free array allocation */
			ZAllocEx<ZAllocAnonSelector>::Free((void**)pAllocationBasePointer);
			this->a = nullptr;
		}
	}

private:
	static void Construct(T* start, T* end)
	{
		for (T i = start; i < end; i++)
		{
			i = T();
		}
	}

	static void Destroy(T* start, T* end)
	{
		for (T i = start; i < end; i++)
		{
			~T();
		}
	}

	void Alloc(size_t uSize)
	{
		this->RemoveAll();

		if (!uSize) return;

		/* Allocate Desired Array Size + 4 bytes */
		PVOID pAlloc = ZAllocEx<ZAllocAnonSelector>::Alloc(sizeof(T) * uSize + sizeof(PVOID));

		/* Assign number of array items to array head */
		*(DWORD*)pAlloc = uSize;

		/* Assign start of real allocated block to array pointer */
		/* We take index 1 because index zero is the array item count */
		this->a = reinterpret_cast<T*>(pAlloc[1]);
	}

	void Realloc(size_t uNewArraySize, int nMode) // Has an extra parameter (ZAllocHelper*) that I think is used for memory profiling the debug build
	{
		size_t uCurArraySize, // current number of max items
			uMaxCountInAllocBlock; // max items that will fit inside of the allocated memory block
		/* Sometimes the above numbers are not the same because memory blocks are often larger than the requested array size */

		uCurArraySize = this->GetCount();

		if (uNewArraySize > uCurArraySize)
		{
			if (this->a)
			{
				/* Grab size of allocation block -- remember, ZAllocEx encodes this at the head of each block */
				uMaxCountInAllocBlock = reinterpret_cast<DWORD*>(this->a)[-2];

				if (uMaxCountInAllocBlock > INT_MAX) // this means its negative since the datatype is unsigned
				{
					uMaxCountInAllocBlock = ~uMaxCountInAllocBlock;
				}

				/* Determine the real number of array item slots based on the allocation block header */
				uMaxCountInAllocBlock = (uMaxCountInAllocBlock - 4) / sizeof(T);
			}
			else
			{
				uMaxCountInAllocBlock = 0;
			}

			/* If the real size of the existing allocation block fits the desired number of array items, do nothing, else allocate new memory */
			if (uNewArraySize > uMaxCountInAllocBlock)
			{
				/* Allocated new block */
				PVOID pNewAllocationBase = ZAllocEx<ZAllocAnonSelector>::Alloc(sizeof(T) * uNewArraySize + sizeof(PVOID));

				/* Increment allocation base to save a spot for the array size encoding */
				pNewAllocationBase += 1;

				if ((nMode & 2) == 0)
				{
					/* Initialize all objects between the current array size and the increased array size in the new block */
					this->Construct(&pNewAllocationBase[uCurArraySize], &pNewAllocationBase[uNewArraySize]);
				}

				if (this->a)
				{
					if ((nMode & 1) == 0)
					{
						/* Copy existing array items into new allocation */
						memcpy(pNewAllocationBase, this->a, sizeof(T) * uCurArraySize);
					}

					/* Free old memory allocation */
					void** pCurrentAllocationBase = reinterpret_cast<void**>(this->a) - 1;
					ZAllocEx<ZAllocAnonSelector>::Free(pCurrentAllocationBase);
				}
				this->a = pNewAllocationBase;
			}
			else if ((nMode & 2) == 0)
			{
				/* Initialize all objects between the current array size and the increased array size in the same block */
				this->Construct(&this->a[uCurArraySize], &this->a[uNewArraySize]);
			}
		}
		else
		{
			/* Destroy all objects between the new count and the old count */
			this->Destroy(&this->a[uNewArraySize], &this->a[uCurArraySize]);
		}

		if (this->a)
		{
			/* Reassign value at array size pointer to match new size */
			DWORD* pdwArraySize = reinterpret_cast<DWORD*>(this->a) - 1;
			*pdwArraySize = uNewArraySize;
		}
	}

	void Reserve(size_t uItems)
	{
		size_t uCurArraySize;
		size_t uMaxCountInAllocBlock;

		if (this->a)
		{
			/* Grab size of allocation block -- remember, ZAllocEx encodes this at the head of each block */
			uMaxCountInAllocBlock = reinterpret_cast<DWORD*>(this->a)[-2];

			if (uMaxCountInAllocBlock > INT_MAX) // this means its negative since the datatype is unsigned
			{
				uMaxCountInAllocBlock = ~uMaxCountInAllocBlock;
			}

			/* Determine the real number of array item slots based on the allocation block header */
			uMaxCountInAllocBlock = (uMaxCountInAllocBlock - 4) / sizeof(T);
		}
		else
		{
			uMaxCountInAllocBlock = 0;
		}

		if (uMaxCountInAllocBlock == uItems) return;

		uCurArraySize = this->GetCount();

		/* Allocate new block */
		PVOID pNewAllocationBase = ZAllocEx<ZAllocAnonSelector>::Alloc(sizeof(T) * uItems + sizeof(PVOID));

		/* Increment allocation base to save a spot for the array size encoding */
		pNewAllocationBase += 1;

		if (this->a)
		{
			/* Copy existing memory into the new allocation */
			memcpy(pNewAllocationBase, this->a, sizeof(T) * uCurArraySize);

			/* Free old memory allocation */
			void** pCurrentAllocationBase = reinterpret_cast<void**>(this->a) - 1;
			ZAllocEx<ZAllocAnonSelector>::Free(pCurrentAllocationBase);
		}

		this->a = pNewAllocationBase;

		/* Reassign value at array size pointer to match new size */
		DWORD* pdwArraySize = reinterpret_cast<DWORD*>(this->a) - 1;
		*pdwArraySize = uCurArraySize;
	}
};

assert_size(sizeof(ZArray<int>), 0x04)