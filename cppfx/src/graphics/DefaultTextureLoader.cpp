#include <cppfx/graphics/DefaultTextureLoader.h>
#include <cppfx/graphics/BmpTextureLoader.h>
#include <cppfx/graphics/PngTextureLoader.h>
#include <cppfx/graphics/SDLImageTextureLoader.h>
#include <cppfx/Exceptions.h>

namespace cppfx
{
	namespace graphics
	{
		DefaultTextureLoader::DefaultTextureLoader(const ref_ptr<Context>& context) :
			multiLoader(new MultiTextureLoader(context))
		{
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL_IMAGE)
			ref_ptr<SDLImageTextureLoader> sdlImageTextureLoader = new SDLImageTextureLoader(context);
			multiLoader->registerLoader("bmp", sdlImageTextureLoader);
			multiLoader->registerLoader("png", sdlImageTextureLoader);
			multiLoader->registerLoader("tga", sdlImageTextureLoader);
			multiLoader->registerLoader("jpg", sdlImageTextureLoader);
			multiLoader->registerLoader("jpeg", sdlImageTextureLoader);
			multiLoader->registerLoader("tif", sdlImageTextureLoader);
			multiLoader->registerLoader("tiff", sdlImageTextureLoader);
#else
			multiLoader->registerLoader("bmp", new BmpTextureLoader(context));
#ifdef CPPFX_USE_LIBPNG
			multiLoader->registerLoader("png", new PngTextureLoader(context));
#endif
#endif
			
		}
		DefaultTextureLoader::~DefaultTextureLoader()
		{
		}
		ref_ptr<Texture2D> DefaultTextureLoader::loadTexture2D(const string& fileName)
		{
			return multiLoader->loadTexture2D(fileName);
		}
	}
}