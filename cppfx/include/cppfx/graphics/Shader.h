#pragma once
#ifndef __CPPFX_GRAPHICS_SHADER_H
#define __CPPFX_GRAPHICS_SHADER_H

#include <cppfx/Object.h>
#include <cppfx/string.h>
#include <cppfx/ref_ptr.h>
#include <cppfx/graphics/Enums.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API Context;
		class CPPFX_API ShaderProgram;
		class CPPFX_API Shader : public Object
		{
		public:
		private:
			Shader(Context* context, const string& source, ShaderType type);
			virtual ~Shader();

			Shader(const Shader&) = delete;
			Shader& operator = (const Shader&) = delete;

			Context* context;
			unsigned int id;

			friend class ShaderProgram;
		};
	}
}

#endif