#include <cppfx/graphics/Texture.h>

namespace cppfx
{
	namespace graphics
	{
		Texture::Texture(Context* context, unsigned target) :
			context(context),
			id(0),
			target(target)
		{
		}
		Texture::~Texture()
		{
			if (id != 0)
				glDeleteTextures(1, &id);
		}

		void Texture::bind()
		{
			glBindTexture(target, id);
		}
	}
}