#include <cppfx/graphics/DefaultTextureLoader.h>
#include <cppfx/graphics/BmpTextureLoader.h>
#include <cppfx/graphics/PngTextureLoader.h>

namespace cppfx
{
	namespace graphics
	{
		DefaultTextureLoader::DefaultTextureLoader(const ref_ptr<Context>& context) :
			multiLoader(new MultiTextureLoader(context))
		{
			multiLoader->registerLoader("bmp", new BmpTextureLoader(context));
#ifdef CPPFX_USE_LIBPNG
			multiLoader->registerLoader("png", new PngTextureLoader(context));
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