#pragma once
#ifndef __CPPFX_GRAPHICS_BMPTEXTURELOADER_H
#define __CPPFX_GRAPHICS_BMPTEXTURELOADER_H

#include <cppfx/graphics/Context.h>
#include <cppfx/weak_ptr.h>

namespace cppfx {
	namespace graphics {
		class CPPFX_API BmpTextureLoader : public Object, public virtual ITextureLoader {
		public:
			BmpTextureLoader(const ref_ptr<graphics::Context>& context);
			virtual ref_ptr<Texture2D> loadTexture2D(const string& fileName) override;
		private:
			BmpTextureLoader(const BmpTextureLoader&) = delete;
			BmpTextureLoader& operator = (const BmpTextureLoader&) = delete;
			weak_ptr<Context> context;
		};
	}
}

#endif