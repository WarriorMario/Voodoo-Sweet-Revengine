#pragma once
#include <type_traits>

template<typename T>
using RemoveRef = std::remove_reference_t<T>;

template<typename T>
constexpr T&& Forward(RemoveRef<T>& arg) noexcept
{
  return std::forward<T>(arg);
}
template<typename T>
constexpr T&& Forward(RemoveRef<T>&& arg) noexcept
{
  return std::forward<T>(arg);
}
template<typename T>
constexpr auto Move(T&& arg) -> decltype(std::move(arg))
{
  return std::move(arg);
}

template<bool Test, typename T = void>
using EnableIf = std::enable_if_t<Test, T>;

