#pragma once
#ifndef __CPPFX_VECTOR_H
#define __CPPFX_VECTOR_H

#include <cppfx/config.h>
#include <cppfx/export.h>

#include <glm/glm.hpp>
#include <cppfx/string.h>

namespace cppfx
{
	typedef glm::vec2 vector2f;
	typedef glm::vec3 vector3f;
	typedef glm::vec4 vector4f;

	typedef glm::vec4 colorf;
	typedef glm::u8vec4 colorub;
	typedef glm::i8vec4 colorb;

	typedef glm::ivec2 vector2i;
	typedef glm::ivec3 vector3i;
	typedef glm::ivec4 vector4i;

	struct CPPFX_API colors
	{
		static const colorf white;
		static const colorf black;
		static const colorf red;
		static const colorf green;
		static const colorf blue;
	};

	template<typename T, typename TT>
	typename T lerp(const T& v0, const T& v1, TT t)
	{
		return (1 - t)*v0 + t*v1;
	}

	inline colorf parseColor(const string& str) {
		return colorf(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

#endif