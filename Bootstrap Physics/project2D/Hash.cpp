#include "Hash.h"

Hash::Hash()
{
}

Hash::~Hash()
{
}

unsigned int Hash::DoHash(const char* val, int size)
{
	if (size <= 0)
		return 0;

	unsigned int result = 0;
	for (int i = 0; i < size; ++i)
	{
		result += val[i] ^ (val[i] << 3 * 8);
		result -= val[i] ^ 0xfbc;
		result = result << 8;
		result ^= val[i] * 3;
	}

	result ^= val[size - 1];
	result *= val[size - 1];
	return result;
}