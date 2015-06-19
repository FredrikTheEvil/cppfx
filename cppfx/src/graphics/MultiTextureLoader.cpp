#include <cppfx/graphics/MultiTextureLoader.h>
#include <cppfx/Exceptions.h>

namespace cppfx
{
	namespace graphics
	{
		MultiTextureLoader::MultiTextureLoader(const ref_ptr<Context>& context)
		{
			
		}
		MultiTextureLoader::~MultiTextureLoader()
		{
			
		}
		void MultiTextureLoader::registerLoader(const string& fileExtension, const ref_ptr<ITextureLoader>& loader)
		{
			loadersByFileExtension[fileExtension] = loader;
		}
		ref_ptr<Texture2D> MultiTextureLoader::loadTexture2D(const string& fileName)
		{
			auto pos = fileName.find_last_of('.');
			if (pos != string::npos)
			{
				string extension = fileName.substr(pos + 1);
				if (!extension.empty()) {
					auto loader = loadersByFileExtension.find(extension);
					if (loader != loadersByFileExtension.end())
						return loader->second->loadTexture2D(fileName);
					throw RuntimeError(string("unknown file extension: ") + extension);
				}
			}
			throw RuntimeError("unable to determine texture file format");
		}
	}
}