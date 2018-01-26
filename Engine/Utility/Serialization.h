#pragma once
#include "VString.h"
#include <stack>

namespace Json
{
	class Value;
}

enum ErrorCodes
{
	FAILURE = 0,
	SUCCESS = 1
};

// class for deserializing data.
// this class will serialize any values added with the Add function and deserialize a file
// a file and put the parameters in the paramters map. you can then fetch all deserialized 
// values using the GetParameter() functions
class Serializer
{
public:
	Serializer(const String& a_rRoot);
	~Serializer();

	ErrorCodes Serialize(const String& filePath);
	ErrorCodes Deserialize(const String& filePath);

	void StepIn(const String& name);
	void StepOut();

	void Add(const String& name, float value);
	void Add(const String& name, bool value);
	void Add(const String& name, double value);
	void Add(const String& name, int value);
	void Add(const String& name, unsigned int value);
	void Add(const String& name, size_t value);
	void Add(const String& name, const String& value);
				   		   
	void Get(const String& name, float& value);
	void Get(const String& name, bool& value);
	void Get(const String& name, double& value);
	void Get(const String& name, int& value);
	void Get(const String& name, unsigned int& value);
	void Get(const String& name, size_t& value);
	void Get(const String& name, String& value);

private:
	inline Json::Value& Root()
	{
		return *m_values.top();
	}
	std::stack<Json::Value*> m_values;
	String m_root;
};