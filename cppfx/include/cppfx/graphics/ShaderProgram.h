#pragma once
#ifndef __CPPFX_GRAPHICS_SHADERPROGRAM_H
#define __CPPFX_GRAPHICS_SHADERPROGRAM_H

#include <cppfx/Object.h>
#include <cppfx/string.h>
#include <cppfx/ref_ptr.h>
#include <cppfx/graphics/Enums.h>
#include <cppfx/graphics/Shader.h>
#include <cppfx/graphics/ShaderProgramParameter.h>

#include <unordered_map>
#include <vector>

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(push)
#	pragma warning( disable : 4251 )
#endif

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API Context;
		class CPPFX_API ShaderProgram : public Object
		{
		public:
			typedef std::vector< ref_ptr<ShaderProgramParameter> >::const_iterator param_iterator;

			ref_ptr<ShaderProgramParameter> getParameter(const string& key);
			param_iterator parameterBegin();
			param_iterator parameterEnd();
		protected:
			virtual ~ShaderProgram();
		private:
			ShaderProgram(Context* context, const string& vertexShader, const string& fragShader);
			Context* context;
			uint32_t id;
			ref_ptr<Shader> vertexShader;
			ref_ptr<Shader> fragmentShader;
			std::vector< ref_ptr<ShaderProgramParameter> > parameters;

			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram& operator = (const ShaderProgram&) = delete;

			friend class Context;
		};
	}
}

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(pop)
#endif

#endif