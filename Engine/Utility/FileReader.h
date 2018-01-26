#pragma once
#include "VString.h"
#include <memory>

struct  FileHandle
{
	std::shared_ptr<char> pHandle;
	size_t size;
};

class FileReader
{
public:
	FileReader() {};
	~FileReader();
	FileHandle Open(const String& file_name);
};