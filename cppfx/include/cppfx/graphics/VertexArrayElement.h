#pragma once
#ifndef __CPPFX_GRAPHICS_VERTEXARRAYELEMENT_H
#define __CPPFX_GRAPHICS_VERTEXARRAYELEMENT_H

#include <cppfx/config.h>
#include <cppfx/export.h>
#include <cppfx/graphics/Enums.h>

namespace cppfx
{
	namespace graphics
	{
		struct CPPFX_API VertexArrayElement
		{
			VertexArrayElement();
			VertexArrayElement(unsigned int size, VertexElementType type, VertexAttributeType attribute, unsigned int buffer, bool normalize);
			VertexArrayElement(const VertexArrayElement& el);
			VertexArrayElement& operator = (const VertexArrayElement& el);

			unsigned int size;
			VertexElementType type;
			VertexAttributeType attribute;
			unsigned int buffer;
			bool normalize;
		};
	}
}

#endif