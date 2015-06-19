#pragma once
#ifndef __CPPFX_WEAKMAP_H
#define __CPPFX_WEAKMAP_H

#include <cppfx/Observer.h>
#include <unordered_map>
#include "ref_ptr.h"
#include "Referenced.h"
#include "string.h"

namespace cppfx
{
	template<typename Key, typename Value, typename std::enable_if<std::is_base_of<Referenced, Value>::value>::type* = nullptr>
	class WeakMap : Observer
	{
	public:
		virtual ~WeakMap() override {
		}
		virtual void ObjectDeleted(void* ptr) override {
			for (auto itr = map.begin(); itr != map.end(); ++itr)
			{
				if (static_cast<void*>(itr->second) == ptr)
				{
					erase(itr->first);
					return;
				}
			}
		}
	public:
		WeakMap()
		{
		}
		void insert(const Key& key, const ref_ptr<Value>& value)
		{
			if (value.valid())
			{
				auto itr = map.find(key);
				if (itr != map.end())
					throw std::runtime_error("key is alread in use");
				map.insert(std::pair<Key, Value*>(key, value.get()));
				value->addObserver(this);
			}
		}
		void erase(const Key& key)
		{
			auto itr = map.find(key);
			if (itr != map.end())
			{
				itr->second->removeObserver(this);
				map.erase(itr);
			}
		}
		bool contains(const Key& key)
		{
			return map.find(key) != map.end();
		}
		bool tryGet(const Key& key, ref_ptr<Value>& value)
		{
			auto itr = map.find(key);
			if (itr != map.end()) {
				value = itr->second;
				return true;
			}
			return false;
		}
	private:
		WeakMap(const WeakMap&) = delete;
		WeakMap& operator = (const WeakMap&) = delete;
		std::unordered_map<Key, Value*> map;
	};
}

#endif