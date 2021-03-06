#pragma once
#ifndef __CPPFX_GRAPHICS_PNGTEXTURELOADER_H
#define __CPPFX_GRAPHICS_PNGTEXTURELOADER_H
#include <cppfx/config.h>
#if defined(CPPFX_USE_LIBPNG) && !defined(__EMSCRIPTEN__)
#include <cppfx/graphics/ITextureLoader.h>
#include <cppfx/graphics/Context.h>
#include <cppfx/weak_ptr.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API PngTextureLoader : public Object, public virtual ITextureLoader
		{
		public:
			PngTextureLoader(const ref_ptr<Context>& context);

			virtual ref_ptr<Texture2D> loadTexture2D(const string& fileName) override;
		private:
			weak_ptr<Context> context;
			virtual ~PngTextureLoader();
		};
	}

}
#endif
#endif