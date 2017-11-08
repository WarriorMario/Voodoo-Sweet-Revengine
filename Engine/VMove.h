#pragma once
#include <type_traits>

template <typename T>
auto Forward(T&& arg) -> decltype(std::forward(arg))
{
	return std::forward(arg);
}
template <typename T>
auto Move(T&& arg) -> decltype(std::move(arg))
{
	return std::move(arg);
}