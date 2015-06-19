#include <cppfx/graphics/Enums.h>
#include <cppfx/graphics/Shader.h>
#include <cppfx/graphics/ShaderProgramParameter.h>
#include <cppfx/vector.h>
#include <glm/gtc/type_ptr.hpp>

namespace cppfx
{
	namespace graphics
	{
		const string& ShaderProgramParameter::getName() const
		{
			return name;
		}

		ShaderProgram* ShaderProgramParameter::getProgram()
		{
			return program;
		}

		void ShaderProgramParameter::setFloat(float x)
		{
			glUniform1f(location, x);
		}

		void ShaderProgramParameter::setVector2f(const vector2f& v)
		{
			glUniform2f(location, v.x, v.y);
		}
		void ShaderProgramParameter::setVector3f(const vector3f& v)
		{
			glUniform3f(location, v.x, v.y, v.z);
		}
		void ShaderProgramParameter::setVector4f(const vector4f& v)
		{
			glUniform4f(location, v.x, v.y, v.z, v.w);
		}

		void ShaderProgramParameter::setInt(int x)
		{
			glUniform1i(location, x);
		}

		void ShaderProgramParameter::setVector2i(const vector2i& v)
		{
			glUniform2i(location, v.x, v.y);
		}

		void ShaderProgramParameter::setVector3i(const vector3i& v)
		{
			glUniform3i(location, v.x, v.y, v.z);
		}

		void ShaderProgramParameter::setVector4i(const vector4i& v)
		{
			glUniform4i(location, v.x, v.y, v.z, v.w);
		}

		void ShaderProgramParameter::setMatrix2(const matrix2f& m)
		{
			glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(m));
		}
		void ShaderProgramParameter::setMatrix3(const matrix3f& m)
		{
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(m));
		}
		void ShaderProgramParameter::setMatrix4(const matrix4f& m)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
		}

		ShaderProgramParameter::ShaderProgramParameter()
		{
		}
	}
}