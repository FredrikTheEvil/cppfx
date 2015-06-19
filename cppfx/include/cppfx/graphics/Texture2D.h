#pragma once
#ifndef __CPPFX_GRAPHICS_TEXTURE2D_H
#define __CPPFX_GRAPHICS_TEXTURE2D_H

#include <cppfx/ref_ptr.h>
#include <cppfx/graphics/Texture.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API Texture2D : public Texture
		{
		public:
			int getWidth() const;
			int getHeight() const;
			MinFilter getMinFilter() const;
			MagFilter getMagFilter() const;
			void setMinFilter(MinFilter minFilter);
			void setMagFilter(MagFilter magFilter);
		protected:
			virtual void bind() override;
		private:
			Texture2D(Context* context, TextureFormat format, int width, int height, TextureDataFormat dataFormat, TextureDataType dataType, const void* data);
			int width;
			int height;
			TextureFormat format;
			MinFilter minFilter;
			bool minFilterDirty;
			MagFilter magFilter;
			bool magFilterDirty;

			Texture2D(const Texture2D&) = delete;
			Texture2D& operator=(const Texture2D&) = delete;

			friend class Context;
		};
	}
}

#endif