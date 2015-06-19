#pragma once
#ifndef __CPPFX_GUI_SPRITEVERTEX_H
#define __CPPFX_GUI_SPRITEVERTEX_H

#include <cppfx/config.h>
#include <cppfx/export.h>
#include <cppfx/vector.h>

namespace cppfx
{
	namespace gui
	{
		struct CPPFX_API SpriteVertex
		{
			SpriteVertex();
			SpriteVertex(const SpriteVertex& v);
			SpriteVertex& operator = (const SpriteVertex& v);

			vector2f pos;
			vector2f uv;
			vector4f color;
		};
	}
}

#endif