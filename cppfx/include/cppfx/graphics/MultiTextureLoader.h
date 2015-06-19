#pragma once
#ifndef __CPPFX_GRAPHICS_MULTITEXTURELOADER_H
#define __CPPFX_GRAPHICS_MULTITEXTURELOADER_H
#include <cppfx/graphics/ITextureLoader.h>
#include <cppfx/graphics/Context.h>
#include <unordered_map>
#include <cppfx/weak_ptr.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API MultiTextureLoader : public Object, public virtual ITextureLoader
		{
		public:
			MultiTextureLoader(const ref_ptr<Context>& context);
			void registerLoader(const string& fileExtension, const ref_ptr<ITextureLoader>& loader);
			virtual ref_ptr<Texture2D> loadTexture2D(const string& fileName) override;
		private:
			weak_ptr<Context> context;
			virtual ~MultiTextureLoader();
			std::unordered_map<string, ref_ptr<ITextureLoader> > loadersByFileExtension;
		};
	}
}

#endif