#pragma once
#include "ZRecyclable.h"

template <typename T, typename U, typename V>
class ZMap
{
public:
	struct _PAIR : ZRecyclable<ZMap<T, U, V>::_PAIR, int, ZMap<T, U, V>::_PAIR>
	{
	private:
		char aPad[0x4]; // vfptr
	public:
		ZMap<T, U, V>::_PAIR* pNext;
		T key;
		U value;
	};

private:
	char aPad[0x4];

public:
	//ZMap<T, U, V>Vtbl* vfptr;
	ZMap<T, U, V>::_PAIR** _m_apTable;
	unsigned int _m_uTableSize;
	unsigned int _m_uCount;
	unsigned int _m_uAutoGrowEvery128;
	unsigned int _m_uAutoGrowLimit;

	U* GetAt(const T* key)
	{
		ZMap<T, U, V>::_PAIR** v3; // esi
		ZMap<T, U, V>::_PAIR* v5; // esi

		v3 = this->_m_apTable;

		if (!v3) return 0;

		v5 = v3[_rotr(*key, 5) % this->_m_uTableSize];

		if (!v5) return 0;

		while (v5->key != *key)
		{
			v5 = v5->pNext;

			if (!v5) return 0;
		}

		// we are gonna skip the copying for now
		//if (value) ZRef<AdditionPsd>::operator=(value, &v5->value);

		return &v5->value;
	}
};