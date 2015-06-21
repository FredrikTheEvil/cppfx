#pragma once
#ifndef __CPPFX_GRAPHICS_SDLIMAGETEXTURELOADER_H
#define __CPPFX_GRAPHICS_SDLIMAGETEXTURELOADER_H
#include <cppfx/config.h>
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL_IMAGE)
#include <cppfx/graphics/ITextureLoader.h>
#include <cppfx/graphics/Context.h>
#include <cppfx/weak_ptr.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API SDLImageTextureLoader : public Object, public virtual ITextureLoader
		{
		public:
			SDLImageTextureLoader(const ref_ptr<Context>& context);
			virtual ref_ptr<Texture2D> loadTexture2D(const string& fileName) override;
		private:
			weak_ptr<Context> context;
			virtual ~SDLImageTextureLoader();
		};
	}
}
#endif
#endif