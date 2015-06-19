#include <cppfx/graphics/ShaderProgram.h>
#include <cppfx/graphics/ShaderProgramParameter.h>
#include <cppfx/graphics/Context.h>

#ifdef CPPFX_USE_GLEW
#include <GL/glew.h>
#endif

namespace cppfx
{
	namespace graphics
	{
		ref_ptr<ShaderProgramParameter> ShaderProgram::getParameter(const string& key)
		{
			auto hash = std::hash<string>()(key);
			for (auto i = parameters.begin(); i != parameters.end(); ++i)
				if ((*i)->hash == hash)
					return *i;
			return nullptr;
		}

		ShaderProgram::param_iterator ShaderProgram::parameterBegin()
		{
			return parameters.cbegin();
		}

		ShaderProgram::param_iterator ShaderProgram::parameterEnd()
		{
			return parameters.cend();
		}

		ShaderProgram::ShaderProgram(Context* context, const string& vertSource, const string& fragSource) :
			context(context)
		{
			ref_ptr<Context> ctx(context);

			ref_ptr<Shader> tmpVertexShader = new Shader(context, vertSource, ShaderType::VERTEX_SHADER);
			ref_ptr<Shader> tmpFragmentShader = new Shader(context, fragSource, ShaderType::FRAGMENT_SHADER);

			id = glCreateProgram();
			glAttachShader(id, tmpVertexShader->id);
			glAttachShader(id, tmpFragmentShader->id);

			glBindAttribLocation(id, GLuint(VertexAttributeType::VERTEX), "v_Vertex");
			glBindAttribLocation(id, GLuint(VertexAttributeType::NORMAL), "v_Normal");
			glBindAttribLocation(id, GLuint(VertexAttributeType::TANGENT), "v_Tangent");
			glBindAttribLocation(id, GLuint(VertexAttributeType::UV1), "v_Uv1");
			glBindAttribLocation(id, GLuint(VertexAttributeType::UV2), "v_Uv2");
			glBindAttribLocation(id, GLuint(VertexAttributeType::COLOR), "v_Color");
			glBindAttribLocation(id, GLuint(VertexAttributeType::BONEINDICES), "v_BoneIndices");
			glBindAttribLocation(id, GLuint(VertexAttributeType::BONEWEIGHTS), "v_BoneWeights");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE0), "v_User0");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE1), "v_User1");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE2), "v_User2");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE3), "v_User3");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE4), "v_User4");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE5), "v_User5");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE6), "v_User6");
			glBindAttribLocation(id, GLuint(VertexAttributeType::USER_ATTRIBUTE7), "v_User7");

			glLinkProgram(id);

			int status = 0;
			glGetProgramiv(id, GL_LINK_STATUS, &status);
			if (status != GL_TRUE)
			{
				int logLength = 0;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
				if (logLength <= 0) {
					glDeleteProgram(id);
					throw std::runtime_error("failed to compile shader: unknown reason");
				}
				char* buffer = new char[logLength];
				GLsizei l;
				glGetProgramInfoLog(id, logLength, &l, buffer);
				string errorLog(buffer, buffer + l);
				delete[] buffer;
				glDeleteProgram(id);
				throw std::runtime_error(errorLog.data());
			}

			int numUniforms = 0;
			glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numUniforms);
			std::vector< ref_ptr<ShaderProgramParameter> > tmpParams;
			char buffer[128];
			for (int i = 0; i < numUniforms; i++)
			{
				GLsizei len;
				GLint size;
				GLenum type;
				glGetActiveUniform(id, i, 128, &len, &size, &type, buffer);
				ref_ptr<ShaderProgramParameter> param = new ShaderProgramParameter();
				string name(buffer, buffer + len);
				param->name = name;
				param->hash = std::hash<string>()(name);
				param->location = glGetUniformLocation(id, buffer);
				param->type = ShaderProgramParameterType(type);
				param->size = size;
				tmpParams.push_back(param);
			}
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::runtime_error(str);
			}
#endif
			vertexShader = tmpVertexShader;
			fragmentShader = tmpFragmentShader;
			parameters = tmpParams;
			context->ref();
		}
		ShaderProgram::~ShaderProgram()
		{
			if (id != 0)
				glDeleteProgram(id);
			context->unref();
		}
	}
}