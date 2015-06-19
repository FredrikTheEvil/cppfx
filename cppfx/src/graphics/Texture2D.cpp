#include <cppfx/graphics/Texture2D.h>
#include <exception>

namespace cppfx
{
	namespace graphics
	{
		int Texture2D::getWidth() const
		{
			return width;
		}

		int Texture2D::getHeight() const
		{
			return height;
		}

		MinFilter Texture2D::getMinFilter() const
		{
			return minFilter;
		}

		MagFilter Texture2D::getMagFilter() const
		{
			return magFilter;
		}

		void Texture2D::setMinFilter(MinFilter minFilter)
		{
			if (minFilter != Texture2D::minFilter) {
				Texture2D::minFilter = minFilter;
				minFilterDirty = true;
			}
		}
		void Texture2D::setMagFilter(MagFilter magFilter)
		{
			if (magFilter != Texture2D::magFilter)
			{
				Texture2D::magFilter = magFilter;
				magFilterDirty = true;
			}
		}
		void Texture2D::bind()
		{
			Texture::bind();
			if (minFilterDirty) {
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GLint(minFilter));
				minFilterDirty = false;
			}
			if (magFilterDirty)
			{
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GLint(magFilter));
				magFilterDirty = false;
			}
		}

		Texture2D::Texture2D(Context* context, TextureFormat format, int width, int height, TextureDataFormat dataFormat, TextureDataType dataType, const void* data) :
			Texture(context, GL_TEXTURE_2D),
			width(width),
			height(height),
			format(format),
			minFilter(MinFilter::LINEAR),
			minFilterDirty(true),
			magFilter(MagFilter::LINEAR),
			magFilterDirty(true)
			
		{
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GLint(format), width, height, 0, GLenum(dataFormat), GLenum(dataType), data);
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::runtime_error(str);
			}
#endif
		}
	}
}