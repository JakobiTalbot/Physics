#pragma once
class Hash
{
public:
	Hash();
	~Hash();

	unsigned int DoHash(const char* val, int size);
};