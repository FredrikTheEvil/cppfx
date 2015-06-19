#include <cppfx/gui/SpriteVertex.h>

namespace cppfx
{
	namespace gui
	{
		SpriteVertex::SpriteVertex() :
			pos(),
			uv(),
			color()
		{
		}
		SpriteVertex::SpriteVertex(const SpriteVertex& v) :
			pos(v.pos),
			uv(v.uv),
			color(v.color)
		{
		}

		SpriteVertex& SpriteVertex::operator=(const SpriteVertex& v)
		{
			pos = v.pos;
			uv = v.uv;
			color = v.color;
			return *this;
		}
	}
}