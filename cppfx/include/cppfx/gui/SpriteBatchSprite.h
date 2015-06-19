#pragma once
#ifndef __CPPFX_GUI_SPRITE_H
#define __CPPFX_GUI_SPRITE_H

#include <cppfx/gui/SpriteVertex.h>

namespace cppfx
{
	namespace gui
	{
		struct CPPFX_API SpriteBatchSprite
		{
			SpriteBatchSprite();
			SpriteBatchSprite(const SpriteBatchSprite& sprite);
			SpriteBatchSprite& operator = (const SpriteBatchSprite& sprite);

			SpriteVertex topLeft;
			SpriteVertex bottomLeft;
			SpriteVertex topRight;
			SpriteVertex bottomRight;
		};
	}
}

#endif