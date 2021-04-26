#pragma once
#include "logger.h"

class ZRefCounted
{
public:

	ZRefCounted()
	{
		this->m_nRef = 0;
		this->m_pPrev = 0;
	}

	virtual ~ZRefCounted() = default;

	union
	{
		volatile long m_nRef;
		ZRefCounted* m_pNext;
	};

	ZRefCounted* m_pPrev;
};

/* not really sure why nexon has this class but this is all it does o__o */
template <class T>
T* ZRefCounted_Alloc()
{
	T* pAlloc = new T();

	return pAlloc;
}

assert_size(sizeof(ZRefCounted), 0x0C);