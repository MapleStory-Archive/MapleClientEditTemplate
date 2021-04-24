#pragma once
#include "logger.h"
#include "ZRefCounted.h"
#include "ZRefCountedDummy.h"
#include "ZRefCountedAccessor.h"

template <class T>
class ZRef : protected ZRefCountedAccessor<T>, protected ZRefCountedAccessor<ZRefCountedDummy<T>>
{
private:
	BYTE gap0[1];
	T* p;

public:
	ZRef()
	{
		
	}

	~ZRef()
	{
		this->ReleaseRaw();
	}

private:
	void ReleaseRaw()
	{
		if (!this->p) return;

		ZRefCounted* t;// = dynamic_cast<ZRefCounted*>(this->p);

		/*if (t)
		{*/
			t = reinterpret_cast<ZRefCounted*>(((char*)this) - 16);
		//}

		if (!InterlockedDecrement(&t->m_nRef))
		{
			InterlockedIncrement(&t->m_nRef);

			t->~ZRefCounted();
		}

		this->p = nullptr;
	}
};

assert_size(sizeof(ZRef<int>), 0x08);