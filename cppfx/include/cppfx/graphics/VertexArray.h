#pragma once
#ifndef __CPPFX_GRAPHICS_VERTEXARRAY_H
#define __CPPFX_GRAPHICS_VERTEXARRAY_H

#include <cppfx/graphics/Buffer.h>
#include <vector>
#include "VertexArrayElement.h"
#include <cppfx/ref_ptr.h>

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(push)
#	pragma warning( disable : 4251 )
#endif

namespace cppfx
{
	namespace graphics
	{
		class CPPFX_API VertexArray : public Object
		{
		public:
			enum class Implementation
			{
				LEGACY,
				VAO
			};
		protected:
			virtual ~VertexArray();
		private:
			VertexArray(Context* context, const std::vector<VertexArrayElement>& elements, const std::vector< ref_ptr<Buffer> >& buffers, ref_ptr<Buffer> indexBuffer);
			Context* context;
			unsigned int id;
			unsigned int prevBoundId;
			std::vector<VertexArrayElement> elements;
			std::vector< ref_ptr<Buffer> > buffers;
			ref_ptr<Buffer> indexBuffer;
			Implementation impl;
			bool legacy;

			// For OpenGL >= 3.1 or with GL_ARB_vertex_array_object support
			void BindCore();
			// For OpenGL >= 3.1 or with GL_ARB_vertex_array_object support
			void UnbindCore();

			// For setting vertex attrib arrays directly (OpenGL >= 2.0)
			void BindLegacy();
			// For setting vertex attrib arrays directly (OpenGL >= 2.0)
			void UnbindLegacy();

			void Bind();
			void Unbind();

			VertexArray(const VertexArray&) = delete;
			VertexArray& operator = (const VertexArray&) = delete;

			friend class Context;
		};
	}
}

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(pop)
#endif

#endif