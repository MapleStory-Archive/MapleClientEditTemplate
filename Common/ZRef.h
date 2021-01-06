#pragma once
#include "ZRecycleable.h"

template <typename T>
struct ZRef
{
private:
	char aPad[4];
public:
	T* p;
};

class ZRefCounted
{
public:

	ZRefCounted()
	{
		this->m_nRef = 0;
		this->m_pPrev = 0;
	}

	virtual ~ZRefCounted()
	{
		//TODO: ?
	}

	union
	{
		long m_nRef;
		ZRefCounted* m_pNext;
	};

	ZRefCounted* m_pPrev;
};

template <typename T>
class ZRefCountedDummy : public ZRefCounted, public ZRecyclable<ZRefCountedDummy<T>, int, T>
{
public:
	T pData;
};

assert_size(sizeof(ZRef<int>), 0x08);
assert_size(sizeof(ZRefCounted), 0x0C);
assert_size(sizeof(ZRefCountedDummy<int>), 0x14);