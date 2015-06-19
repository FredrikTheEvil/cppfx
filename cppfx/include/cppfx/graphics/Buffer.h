#pragma once
#ifndef __CPPFX_GRAPHICS_BUFFER_H
#define __CPPFX_GRAPHICS_BUFFER_H

#include <cppfx/Referenced.h>
#include <cppfx/graphics/Enums.h>
#include <cppfx/Object.h>

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(push)
#	pragma warning( disable : 4251 )
#endif

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API Context;
		class CPPFX_API VertexArray;
		class CPPFX_API Buffer : public Object
		{			
		public:
			void setData(size_t size, const void* data);
			void setSubData(size_t offset, size_t size, const void* data);
			void getSubData(size_t offset, size_t count, const void* data);
			void getSubData(size_t offset, size_t count, void* buffer);
			size_t size() const;
		protected:
			virtual ~Buffer();
		private:
			Buffer(Context* ctx, size_t size, const void* data, BufferUsage usage, BufferTarget target);
			Context* _context;

			unsigned int _id;

			size_t _size;

			BufferUsage _usage;
			BufferTarget _target;

			Buffer(const Buffer&) = delete;
			Buffer& operator = (const Buffer&) = delete;

			friend class Context;
			friend class VertexArray;
		};
	}
	
}

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(pop)
#endif

#endif