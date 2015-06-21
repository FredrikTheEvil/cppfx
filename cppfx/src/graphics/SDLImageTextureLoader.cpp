#include <cppfx/graphics/SDLImageTextureLoader.h>
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL_IMAGE)
#include <SDL_image.h>
#include <cppfx/Exceptions.h>

namespace cppfx 
{
	namespace graphics
	{
		SDLImageTextureLoader::SDLImageTextureLoader(const ref_ptr<Context>& context) :
			context(context)
		{
		}
		SDLImageTextureLoader::~SDLImageTextureLoader()
		{
		}

		static bool s_sdlImageInitialized = false;

		ref_ptr<Texture2D> SDLImageTextureLoader::loadTexture2D(const string& fileName)
		{
			if (!s_sdlImageInitialized)
				IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
			auto surface = IMG_Load(fileName.data());
			if (surface == nullptr)
				throw new RuntimeError("failed to load texture: " + fileName);

			TextureFormat format = TextureFormat::RGBA8;
			TextureDataFormat dataFormat = TextureDataFormat::RGBA;
			TextureDataType dataType = TextureDataType::UNSIGNED_BYTE;

			if (surface->format->BytesPerPixel == 3) {
				format = TextureFormat::RGB8;
				dataFormat = TextureDataFormat::RGB;
			}
			else if (surface->format->BytesPerPixel != 4) {
				SDL_FreeSurface(surface);
				throw RuntimeError("invalid texture format for file: " + fileName);
			}
			ref_ptr<Texture2D> texture = context->createTexture2D(format, surface->w, surface->h, dataFormat, dataType, surface->pixels);
			SDL_FreeSurface(surface);
			return texture;
		}
	}
}
#endif