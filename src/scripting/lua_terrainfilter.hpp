/*
	Copyright (C) 2018 - 2025
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#pragma once

#include <string>
#include <memory>
#include <map>
#include <set>
#include "map/map.hpp"

struct lua_State;
class filter_impl;
namespace lua_mapgen
{
	class filter
	{
	public:
		/**
		 * @a a lua table with the following attributes
		 * [1]: the filter table,
		 * [2]: attributeslocation_sets
		 */
		explicit filter(lua_State* L, int data_index, int res_index = 0);

		//impl_ may contain pointers to known_sets_, copycontructing this will result in segfaults.
		filter(const filter&) = delete;
		filter& operator=(const filter&) = delete;
		//moving is ok though.
		filter(filter&&) noexcept = default;
		filter& operator=(filter&&) noexcept = default;

		~filter();

		bool matches(const gamemap_base& m, map_location l) const;
		//todo: add a clear cache function.
	private:
		std::map<std::string, std::set<map_location>> known_sets_;
		std::unique_ptr<filter_impl> impl_;
	};

	using filter_ptr = std::unique_ptr<filter, std::function<void(filter*)>>;

	std::string register_filter_metatables(lua_State *L);
}

bool luaW_is_mgfilter(lua_State* L, int index);

lua_mapgen::filter* luaW_to_mgfilter(lua_State *L, int index);

lua_mapgen::filter_ptr luaW_check_mgfilter(lua_State *L, int index, bool allow_compile = false);

void lua_mgfilter_setmetatable(lua_State *L);

int intf_terrainfilter_create(lua_State *L);

int intf_mg_get_locations(lua_State* L);
int intf_mg_get_tiles_radius(lua_State* L);

namespace lua_terrainfilter {
	std::string register_metatables(lua_State *L);
}
