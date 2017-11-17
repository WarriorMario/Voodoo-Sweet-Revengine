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

template <typename T, typename Tuple>
struct Index;
template <typename T, typename... Types>
struct Index<T, Tuple<T, Types...>>
{
  static constexpr size_t value = 0;
};
template <typename T, typename U, typename... Types>
struct Index<T, Tuple<U, Types...>>
{
  static constexpr size_t value = 1 + Index<T, Tuple<Types...>>::value;
};
template<typename T, typename... Types>
constexpr size_t IndexValue = Index<T, Types...>::value;

#include <VTraits.h>

template<size_t I = 0, typename... Types, typename Callable, typename... Args>
auto ForEach(Tuple<Types...>&, Callable, Args&&...) -> typename EnableIf<I == sizeof...(Types)>
{}
template<size_t I = 0, typename... Types, typename Callable, typename... Args>
auto ForEach(Tuple<Types...>& tuple, Callable callable, Args&&... args) -> typename EnableIf<I < sizeof...(Types)>
{
  callable(Get<I>(tuple), Forward<Args>(args)...);
  ForEach<I + 1>(tuple, callable, Forward<Args>(args)...);
}

template<size_t I = 0, typename... Types, typename Callable, typename... Args>
auto OnElement(Tuple<Types...>&, size_t, Callable, Args&&...) -> typename EnableIf<I == sizeof...(Types)>
{}
template<size_t I = 0, typename... Types, typename Callable, typename... Args>
auto OnElement(Tuple<Types...>& tuple, size_t index, Callable callable, Args&&... args) -> typename EnableIf<I < sizeof...(Types)>
{
  if(I == index)
  {
    callable(Get<I>(tuple), Forward<Args>(args)...);
  }
  OnElement<I + 1>(tuple, index, callable, Forward<Args>(args)...);
}
