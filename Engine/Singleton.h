#pragma once

#include <cassert>
#include <cstdint>

// ********************************************************************************
template<typename T>
void MakeSingleton();
template<typename T>
void DestroySingleton();

// ********************************************************************************
template<typename T>
class Singleton
{
public:
	static T& Get()
	{
		assert(instance);
		return *instance;
	}

protected:
	Singleton()
	{
		assert(!instance);

		uintptr_t offset = (uintptr_t)(T*)1 - (uintptr_t)(Singleton*)(T*)1;
		instance = reinterpret_cast<T*>((char*)this + offset);
	}
	~Singleton()
	{
		assert(instance);
		instance = nullptr;
	}

private:
	static T* instance;

	friend void MakeSingleton<T>();
	friend void DestroySingleton<T>();
};

// ********************************************************************************
template<typename T>
T* Singleton<T>::instance = nullptr;

// ********************************************************************************
template<typename T>
void MakeSingleton()
{
	assert(!T::instance && "Singleton instance already exists");
	new T();
}
template<typename T>
void DestroySingleton()
{
	assert(T::instance && "Singleton instance doesn't exists");
	delete T::instance;
}
