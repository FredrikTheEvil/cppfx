#pragma once
#ifndef __CPPFX_GRAPHICS_ITEXTURELOADER_H
#define __CPPFX_GRAPHICS_ITEXTURELOADER_H

#include <cppfx/Referenced.h>
#include <cppfx/graphics/Texture2D.h>
#include <cppfx/string.h>
#include <cppfx/ref_ptr.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API ITextureLoader : public virtual Referenced
		{
		public:
			virtual ref_ptr<Texture2D> loadTexture2D(const string& fileName) = 0;
		protected:
			ITextureLoader();
			virtual ~ITextureLoader();
		private:
			ITextureLoader(const ITextureLoader&) = delete;
			ITextureLoader& operator = (const ITextureLoader&) = delete;
		};
	}
}

#endif