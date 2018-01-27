#include "Utility\Serialization.h"
#include "Utility\FileWriter.h"
#include "Utility\FileReader.h"

#include "Utility\Json\json.h"

// serializable structs
#include "Serialization.h"

//-----------------------------------------------
// File IO

Serializer::Serializer(const String& a_rRoot)
	:
	m_root(a_rRoot)
{
	m_values.push(new Json::Value());
	StepIn(a_rRoot);
}

Serializer::~Serializer()
{
	while (m_values.size() > 1)
	{
		m_values.pop();
	}
	delete m_values.top();
	m_values.pop();
}

ErrorCodes Serializer::Serialize(const String& filePath)
{
	FileWriter writer;
	if (true != writer.OpenEmpty(filePath))
	{
		return ErrorCodes::FAILURE;
	}

	Json::StyledWriter JsonWiter;

	// Restore root
	if (m_values.empty() == true)
	{
		return ErrorCodes::FAILURE;
	}
	while (m_values.size() > 1)
	{
		m_values.pop();
	}

	// Open root
	String buf = JsonWiter.write(Root());
	size_t size = buf.size();

	writer.Write((void*)buf.c_str(), size, 0);
	writer.Close();

	// Step back into root
	StepIn(m_root.c_str());

	return ErrorCodes::SUCCESS;
}

ErrorCodes Serializer::Deserialize(const String& filePath)
{
	FileReader reader;
	FileHandle handle = reader.Open(filePath);

	if (handle.pHandle == nullptr)
	{
		return ErrorCodes::FAILURE;
	}

	Json::Reader Jsonreader;

	// Clear stack
	while (m_values.size() > 1)
	{
		m_values.pop();
	}

	// Write from root
	if (Jsonreader.parse(handle.pHandle.get(), handle.pHandle.get() + handle.size, Root()) == false)
	{
		return ErrorCodes::FAILURE;
	}

	// Step back into root
	//StepIn(m_root.c_str());

	return ErrorCodes::SUCCESS;
}

//--------------------------------------------
// step functions

void Serializer::StepIn(const String& name)
{
	Json::Value* pNewRoot = &(Root())[name];
	m_values.push(pNewRoot);
}

void Serializer::StepInArray(int idx)
{
	Json::Value* pNewRoot = &(Root())[idx];
	m_values.push(pNewRoot);
}

void Serializer::StepOut()
{
	// Preserve absolute root
	if (m_values.size() == 1) return;

	m_values.pop();
}

//--------------------------------------------
// Containers

void Serializer::Add(const String& name, float value)
{
	Root()[name] = value;
}

void Serializer::Add(const String& name, bool value)
{
	Root()[name] = value;
}

void Serializer::Add(const String& name, double value)
{
	Root()[name] = value;
}

void Serializer::Add(const String& name, int value)
{
	Root()[name] = value;
}

void Serializer::Add(const String & name, unsigned int value)
{
}

void Serializer::Add(const String& name, size_t value)
{
	Root()[name] = value;
}

void Serializer::Add(const String& name, const String& value)
{
	Root()[name] = value;
}


//---------------------------------------------------------
// Getters

void Serializer::Get(const String& name, float& value)
{
	value = Root()[name].asFloat();
}

void Serializer::Get(const String& name, bool& value)
{
	value = Root()[name].asBool();
}

void Serializer::Get(const String& name, double& value)
{
	value = Root()[name].asDouble();
}

void Serializer::Get(const String& name, int& value)
{
	value = Root()[name].asInt();
}

void Serializer::Get(const String & name, unsigned int& value)
{
	value = Root()[name].asUInt();
}

void Serializer::Get(const String& name, size_t& value)
{
	value = (size_t)Root()[name].asInt64();
}

void Serializer::Get(const String& name, String& value)
{
	value = Root()[name].asCString();
}

void Serializer::GetAtIdx(int idx, int & value)
{
	value = Root()[idx].asInt();
}
