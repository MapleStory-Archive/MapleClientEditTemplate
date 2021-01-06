#pragma once
#include "logger.h"

template <typename T, typename U, typename V>
class ZRecyclable
{
private:
	char aPad[0x4];
};

assert_size(sizeof(ZRecyclable<long, long, long>), 0x4);