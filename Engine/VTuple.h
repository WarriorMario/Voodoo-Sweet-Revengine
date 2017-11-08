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
  return std::get<T>(tuple);
}

template<size_t index, typename... Types>
constexpr auto Get(const Tuple<Types...>& tuple) noexcept -> const std::tuple_element_t<index, Tuple<Types...>>&
{
  return std::get<index>(tuple);
}
template<size_t index, typename... Types>
constexpr auto Get(Tuple<Types...>& tuple) noexcept -> std::tuple_element_t<index, Tuple<Types...>>&
{
  return std::get<index>(tuple);
}
