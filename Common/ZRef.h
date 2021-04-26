#pragma once
#include "logger.h"
#include "ZRefCounted.h"
#include "ZRefCountedDummy.h"
#include "ZRefCountedAccessor.h"
#include <type_traits>

// ZRef is a smart pointer wrapper class that MapleStory uses to manage memory.
// There's the main ZRef template, which wraps any pointer in a ZRefCountedDummy shell
// And then there's the ZRef<ZRefCounted> specialization template that takes all objects
//	that are already a ZRefCounted object. There are many of these. A significant amount of
//	MapleStory classes are objects derived from ZRefCounted.

template <class T>
class ZRef : protected ZRefCountedAccessor<T>, protected ZRefCountedAccessor<ZRefCountedDummy<T>>
{
private:
	BYTE gap0[1];
	T* p;

public:
	ZRef()
	{
		this->p = nullptr;
	}

	ZRef(T* pT, BOOL bAddRef)
	{
		if (!pT || !*pT) // TODO determine what this second conditional is really doing in IDA
		{
			this->p = nullptr;
		}
		else
		{
			this->p = pT;

			if (bAddRef)
			{
				ZRefCounted* pBase = this->GetBase();

				InterlockedIncrement(&pBase->m_nRef); // (this->p - 12)
			}
		}
	}

	ZRef(ZRef<T>* r)
	{
		ZRefCounted* pBase;

		this->p = r->p;

		if (r->p)
		{
			ZRefCounted* pBase = r->GetBase();

			InterlockedIncrement(&pBase->m_nRef); // (this->p - 12)
		}
	}

	~ZRef()
	{
		this->ReleaseRaw();
	}

	void Alloc()
	{
		this->ReleaseRaw();

		/* is_base_of was released in c++11, so maple did this some other way */
		if (std::is_base_of<ZRefCounted, T>())
		{
			ZRefCounted* pAlloc = reinterpret_cast<ZRefCounted*>(ZRefCounted_Alloc<T>());

			pAlloc->m_nRef = 1;
			this->p = reinterpret_cast<T*>(pAlloc);
		}
		else
		{
			ZRefCountedDummy<T>* pAlloc = ZRefCounted_Alloc<ZRefCountedDummy<T>>();

			pAlloc->m_nRef = 1;
			this->p = &pAlloc->t;
		}
	}

private:
	void ReleaseRaw()
	{
		if (!this->p) return;

		ZRefCounted* pBase = this->GetBase();

		if (!InterlockedDecrement(&pBase->m_nRef))
		{
			InterlockedIncrement(&pBase->m_nRef);

			delete p; // if (v3) (**v3)(v3, 1);
		}

		this->p = nullptr;
	}

	ZRefCounted* GetBase()
	{
		ZRefCounted* pBase;

		/* is_base_of was released in c++11, so maple did this some other way */
		if (std::is_base_of<ZRefCounted, T>())
		{
			pBase = reinterpret_cast<ZRefCounted*>(this->p);
		}
		else
		{
			pBase = reinterpret_cast<ZRefCounted*>(((char*)this) - (sizeof(ZRefCountedDummy<T>) - sizeof(T)));

			static_assert(sizeof(ZRefCountedDummy<T>) - sizeof(T) == 16, "Size is not expected value");
		}

		return pBase;
	}
};

assert_size(sizeof(ZRef<int>), 0x08);