#pragma once
#include "logger.h"

class ZFakeStatAvBuff { };

class ZRecyclableStatic
{
public:
	struct CallBack
	{
	public:
		ZRecyclableStatic::CallBack* m_pNext;

	protected:
		virtual ~CallBack() = default;
	};

protected:
	ZRecyclableStatic::CallBack* m_pHead;
};

assert_size(sizeof(ZFakeStatAvBuff), 0x1)
assert_size(sizeof(ZRecyclableStatic::CallBack), 0x8)
assert_size(sizeof(ZRecyclableStatic), 0x4)