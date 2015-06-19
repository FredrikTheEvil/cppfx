#include <cppfx/graphics/Buffer.h>
#include <cppfx/graphics/Context.h>

#ifdef CPPFX_USE_GLEW
#include <GL/glew.h>
#endif

#include <stdexcept>

namespace cppfx
{
	namespace graphics
	{
		inline GLenum GetTargetBinding(GLenum tgt) {
			if (tgt == GLenum(BufferTarget::INDEX_BUFFER))
				return GL_ELEMENT_ARRAY_BUFFER_BINDING;
			else if (tgt == GLenum(BufferTarget::PIXEL_PACK_BUFFER))
				return GL_PIXEL_PACK_BUFFER_BINDING;
			else if (tgt == GLenum(BufferTarget::PIXEL_UNPACK_BUFFER))
				return GL_PIXEL_UNPACK_BUFFER_BINDING;
			return GL_ARRAY_BUFFER_BINDING;
		}

		struct BufferBindLock {
			BufferBindLock(unsigned int buffer, GLenum target) :
				target(target)
			{
				GLenum targetBinding = GetTargetBinding(target);
				glGetIntegerv(targetBinding, &binding);
				glBindBuffer(target, buffer);
			}
			~BufferBindLock() {
				glBindBuffer(target, binding);
			}
			GLenum target;
			int binding = 0;
		};

		Buffer::Buffer(Context* context, size_t size, const void* data, BufferUsage usage, BufferTarget hint) :
			_context(context),
			_size(size),
			_usage(usage),
			_target(hint)
		{
			context->ref();
			glGenBuffers(1, &_id);
			{
				BufferBindLock bind(_id, GLenum(hint));
				glBufferData(GLenum(hint), size, data, GLenum(usage));
			}
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::runtime_error(str);
			}
#endif
		}
		Buffer::~Buffer() {
			glDeleteBuffers(1, &_id);
			_context->unref();
		}

		void Buffer::setData(size_t size, const void* data) {
			BufferBindLock bind(_id, GLenum(_target));
			glBufferData(GLenum(_target), size, data, GLenum(_usage));
			_size = size;
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::runtime_error(str);
			}
#endif
		}

		void Buffer::setSubData(size_t offset, size_t size, const void* data)
		{
			BufferBindLock bind(_id, GLenum(_target));
			glBufferSubData(GLenum(_target), offset, size, data);
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::runtime_error(str);
			}
#endif
		}

		void Buffer::getSubData(size_t offset, size_t count, const void* data) {
			size_t  total = offset + count;
			if (total > _size)
				throw std::out_of_range("");

			BufferBindLock bind(_id, GLenum(_target));
			glBufferSubData(GLenum(_target), offset, count, data);
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::runtime_error(str);
			}
#endif
		}
		void Buffer::getSubData(size_t offset, size_t count, void* buffer) {
			size_t  total = offset + count;
			if (total > _size)
				throw std::out_of_range("");
			BufferBindLock bind(_id, GLenum(_target));
			glGetBufferSubData(GLenum(_target), offset, count, buffer);
#ifndef NDEBUG
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				const char * str = reinterpret_cast<const char*>(glewGetErrorString(err));
				throw std::runtime_error(str);
			}
#endif
		}

		size_t Buffer::size() const {
			return _size;
		}
	}
}