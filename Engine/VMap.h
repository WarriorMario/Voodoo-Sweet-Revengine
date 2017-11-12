#pragma once
#include <map>
#include <unordered_map>

template<typename Key,typename Val>
using Map = std::map<Key, Val>;

template<typename Key, typename Val>
using UnorderedMap = std::unordered_map<Key, Val>;