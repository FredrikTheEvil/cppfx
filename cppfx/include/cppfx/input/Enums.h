#pragma once
#ifndef __CPPFX_INPUT_ENUMS_H
#define __CPPFX_INPUT_ENUMS_H

#include <cppfx/config.h>

#ifndef BITMASK
#define BITMASK(EnumType) \
	inline EnumType operator | (EnumType x, EnumType y) { \
		return EnumType(std::underlying_type<EnumType>::type(x) | std::underlying_type<EnumType>::type(y)); \
										} \
	inline bool operator & (EnumType x, EnumType y) { \
		return (std::underlying_type<EnumType>::type(x) | std::underlying_type<EnumType>::type(y)) != 0; \
										}
#endif

namespace cppfx
{
	namespace input
	{
		enum class Keycode
		{
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z
		};
		


		enum class KeyModifier
		{
			NONE = 0,
			LEFT_CTRL         = 1 << 0,
			RIGHT_CTRL        = 1 << 1,
			LEFT_ALT          = 1 << 2,
			RIGHT_ALT         = 1 << 3,
			LEFT_SHIFT        = 1 << 4,
			RIGHT_SHIFT       = 1 << 5,
			LEFT_APPLICATION  = 1 << 4,
			RIGHT_APPLICATION = 1 << 5,
			
			CTRL        = LEFT_CTRL        | RIGHT_CTRL,
			ALT         = LEFT_ALT         | RIGHT_ALT,
			SHIFT       = LEFT_SHIFT       | RIGHT_SHIFT,
			APPLICATION = LEFT_APPLICATION | RIGHT_APPLICATION
		};
		BITMASK(KeyModifier)

		enum class MouseButton
		{
			LEFT = 0,
			RIGHT = 1,
			MIDDLE = 2,
			BUTTON_0,
			BUTTON_1,
			BUTTON_2,
			BUTTON_3,
			BUTTON_4
		};
	}
}

#endif