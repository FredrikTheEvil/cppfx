#pragma once
#ifndef __CPPFX_GRAPHICS_SHADERPROGRAMPARAMETER_H
#define __CPPFX_GRAPHICS_SHADERPROGRAMPARAMETER_H

#include <cppfx/Object.h>
#include <cppfx/string.h>
#include <cppfx/vector.h>
#include <cppfx/matrix.h>

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API ShaderProgram;
		class CPPFX_API ShaderProgramParameter : public Object
		{
		public:
			const string& getName() const;
			ShaderProgram * getProgram();

			void setFloat(float x);
			void setVector2f(const vector2f& v);
			void setVector3f(const vector3f& v);
			void setVector4f(const vector4f& v);
			void setInt(int x);
			void setVector2i(const vector2i& v);
			void setVector3i(const vector3i& v);
			void setVector4i(const vector4i& v);
			void setMatrix2(const matrix2f& m);
			void setMatrix3(const matrix3f& m);
			void setMatrix4(const matrix4f& m);
		private:
			ShaderProgramParameter();

			size_t hash;
			string name;
			ShaderProgram* program;
			int location;
			ShaderProgramParameterType type;
			int size;

			ShaderProgramParameter(const ShaderProgramParameter&) = delete;
			ShaderProgramParameter& operator = (const ShaderProgramParameter&) = delete;

			friend class ShaderProgram;
		};
	}
}

#endif