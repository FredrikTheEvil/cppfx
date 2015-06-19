#include <cppfx/gui/SpriteBatchSprite.h>

namespace cppfx
{
	namespace gui
	{
		SpriteBatchSprite::SpriteBatchSprite() :
			topLeft(),
			bottomLeft(),
			topRight(),
			bottomRight()
		{
		}
		SpriteBatchSprite::SpriteBatchSprite(const SpriteBatchSprite& sprite) :
			topLeft(sprite.topLeft),
			bottomLeft(sprite.bottomLeft),
			topRight(sprite.topRight),
			bottomRight(sprite.bottomRight)
		{
		}
		SpriteBatchSprite& SpriteBatchSprite::operator=(const SpriteBatchSprite& sprite)
		{
			topLeft = sprite.topLeft;
			bottomLeft = sprite.bottomLeft;
			topRight = sprite.topRight;
			bottomRight = sprite.bottomRight;
			return *this;
		}
	}
}