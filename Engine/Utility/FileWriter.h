#pragma once
#include "VString.h"

class FileWriter
{
public:
	FileWriter() {};
	FileWriter(const String& a_rFileName);
	~FileWriter();

	const bool Open(const String& a_rFileName);
	const bool Close();
	const bool Write(void* a_pBuf, size_t a_size, size_t a_pos);
	const bool Append(void* a_pBuf, size_t a_size);
	const bool OpenEmpty(const String& a_rFileName);

private:
	FILE * m_pFile;
	size_t m_FileSize;
};