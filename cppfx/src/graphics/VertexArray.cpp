#include <cppfx/graphics/VertexArray.h>
#include <cppfx/graphics/Context.h>
#include <cppfx/Exceptions.h>

#ifdef CPPFX_USE_GLEW
#include <GL/glew.h>
#endif

namespace cppfx
{
	namespace graphics
	{
		VertexArray::VertexArray(Context* context, const std::vector<VertexArrayElement>& elements, const std::vector< ref_ptr<Buffer> >& buffers, ref_ptr<Buffer> indexBuffer) :
			context(context),
			indexBuffer(indexBuffer)
		{
			auto majorVersion = context->getMajorVersion();
			auto minorVersion = context->getMinorVersion();

			if (majorVersion > 3) impl = Implementation::VAO;
			else if (majorVersion == 3 && minorVersion >= 1)
				impl = Implementation::VAO;
			else if (context->extensionAvailable("GL_ARB_vertex_array_object"))
				impl = Implementation::VAO;
			else
				impl = Implementation::LEGACY;

			context->ref();

			unsigned int maxBuffer = 0;

			for (size_t i = 0; i < elements.size(); i++) {
				if (elements[i].buffer > maxBuffer)
					maxBuffer = elements[i].buffer;
			}
			if (buffers.size() < maxBuffer + 1)
				throw RuntimeError("not enough buffer units available");

			VertexArray::elements = std::vector<VertexArrayElement>(elements.begin(), elements.end());
			VertexArray::buffers = std::vector< ref_ptr< Buffer> >(buffers.begin(), buffers.end());

			for (size_t i = 0; i < buffers.size(); i++)
				if (buffers[i] == nullptr)
					throw new RuntimeError("buffer null reference");

			if (impl == Implementation::VAO)
			{
				glGenVertexArrays(1, &id);
				glBindVertexArray(id);
				BindLegacy();
				glBindVertexArray(0);
			}
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw RuntimeError(str);
			}
#endif
		}
		VertexArray::~VertexArray() {
			if (impl == Implementation::VAO)
			{
				glDeleteVertexArrays(1, &id);
			}

			context->unref();
		}

		void VertexArray::BindLegacy() {
			if (indexBuffer.valid())
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->_id);
			else
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			std::vector<int> bufferStrides;
			std::vector<int> elementOffsets = std::vector<int>(elements.size());

			for (size_t i = 0; i < buffers.size(); i++) {
				if (buffers[i] == nullptr)
					throw new RuntimeError("buffer null reference");
				int sz = 0;
				int offset = 0;
				for (size_t j = 0; j < elements.size(); j++) {
					if (elements[j].buffer != i)
						continue;
					elementOffsets[j] = sz;
					int elementSize = 0;
					if (elements[j].type == VertexElementType::FLOAT || elements[j].type == VertexElementType::UNSIGNED_INT)
						elementSize = 4 * elements[j].size;
					else if (elements[j].type == VertexElementType::UNSIGNED_BYTE)
						elementSize = elements[j].size;
					else if (elements[j].type == VertexElementType::UNSIGNED_SHORT)
						elementSize = 2 * elements[j].size;
					sz += elementSize;
				}
				if (sz == 0) {
				}
				bufferStrides.push_back(sz);
			}
			for (size_t i = 0; i < buffers.size(); i++) {
				glBindBuffer(GL_ARRAY_BUFFER, buffers[i]->_id);
				for (size_t j = 0; j < elements.size(); j++) {
					if (elements[j].buffer != i)
						continue;
					auto stride = bufferStrides[i];
					auto offset = elementOffsets[j];
					glVertexAttribPointer(GLuint(elements[j].attribute), elements[j].size, GLenum(elements[j].type), elements[j].normalize ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<const void*>(offset));
					glEnableVertexAttribArray(GLuint(elements[j].attribute));
				}
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw RuntimeError(str);
			}
#endif
		}
		void VertexArray::UnbindLegacy() {
			for (size_t i = 0; i < elements.size(); i++) {
				glDisableVertexAttribArray(GLuint(elements[i].attribute));
			}
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw RuntimeError(str);
			}
#endif
		}

		void VertexArray::BindCore()
		{
			glBindVertexArray(id);
			if (indexBuffer.valid())
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->_id);
			else
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		void VertexArray::UnbindCore()
		{
			glBindVertexArray(0);
		}

		void VertexArray::Bind() {
			if (impl == Implementation::VAO)
				BindCore();
			else
				BindLegacy();

#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw RuntimeError(str);
			}
#endif
		}
		void VertexArray::Unbind() {
			if (impl == Implementation::VAO)
				UnbindCore();
			else
				BindCore();
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw RuntimeError(str);
			}
#endif
		}
	}
}