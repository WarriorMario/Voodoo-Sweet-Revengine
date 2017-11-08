#pragma once
#include <tuple>
template<typename... Types>
using Tuple = std::tuple<Types...>;

template<typename T, typename... Types>
constexpr const T& Get(const Tuple<Types...>& tuple) noexcept
{
	return std::get<T>(tuple);
}
template<typename T, typename... Types>
constexpr T& Get(Tuple<Types...>& tuple) noexcept
{
	return std::get<T, Types...>(tuple);
}