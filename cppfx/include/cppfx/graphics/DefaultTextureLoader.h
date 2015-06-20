#pragma once
#ifndef __CPPFX_GRAPHICS_DEFAULTTEXTURELOADER_H
#define __CPPFX_GRAPHICS_DEFAULTTEXTURELOADER_H
#include <cppfx/graphics/MultiTextureLoader.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API DefaultTextureLoader : public Object, public virtual ITextureLoader {
		public:
			DefaultTextureLoader(const ref_ptr<Context>& context);
			virtual ref_ptr<Texture2D> loadTexture2D(const string& fileName) override;
		private:
			virtual ~DefaultTextureLoader();
			ref_ptr<MultiTextureLoader> multiLoader;
		};
	}
}

#endif