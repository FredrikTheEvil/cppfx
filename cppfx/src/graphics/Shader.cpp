#include <cppfx/graphics/Shader.h>
#include <cppfx/graphics/Context.h>

namespace cppfx
{
	namespace graphics
	{
		Shader::Shader(Context* context, const string& source, ShaderType type) :
			context(context)
		{
			ref_ptr<Context> ctx(context);
			id = glCreateShader(GLenum(type));
			auto str = get_string_buffer(source);
			GLint len[] = { source.size() };
			glShaderSource(id, 1, &str, len);
			glCompileShader(id);

			int status = 0;
			glGetShaderiv(id, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE)
			{
				int logLength = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
				if (logLength <= 0) {
					glDeleteShader(id);
					throw std::exception("failed to compile shader: unknown reason");
				}
				char* buffer = new char[logLength];
				GLsizei l;
				glGetShaderInfoLog(id, logLength, &l, buffer);
				string errorLog(buffer, buffer + l);
				delete[] buffer;
				glDeleteShader(id);
				throw std::exception(errorLog.data());
			}
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::exception(str);
			}
#endif
			context->ref();
		}
		Shader::~Shader()
		{
			if (id != 0)
				glDeleteShader(id);
			context->unref();
		}
	}
}