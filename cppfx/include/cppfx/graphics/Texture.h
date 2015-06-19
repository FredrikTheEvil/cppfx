#pragma once
#ifndef __CPPFX_GRAPHICS_TEXTURE_H
#define __CPPFX_GRAPHICS_TEXTURE_H

#include <cppfx/Object.h>
#include <cppfx/graphics/Enums.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API Context;
		class CPPFX_API Texture : public Object
		{
		public:
		protected:
			Texture(Context* context, unsigned int target);
			virtual ~Texture();
			virtual void bind();
			Context* context;
			unsigned int id;
			unsigned int target;			

			friend class Context;
		private:
			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;
		};
	}
}

#endif