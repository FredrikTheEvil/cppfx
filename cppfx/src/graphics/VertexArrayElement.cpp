#include <cppfx/graphics/VertexArrayElement.h>

namespace cppfx {
	namespace graphics
	{
		VertexArrayElement::VertexArrayElement() :
			size(4),
			type(VertexElementType::FLOAT),
			attribute(VertexAttributeType::VERTEX),
			buffer(0),
			normalize(false)
		{
		}
		VertexArrayElement::VertexArrayElement(unsigned int size, VertexElementType type, VertexAttributeType attribute, unsigned int buffer, bool normalize) :
			size(size),
			type(type),
			attribute(attribute),
			buffer(buffer),
			normalize(normalize)
		{
		}

		VertexArrayElement::VertexArrayElement(const VertexArrayElement& el) :
			size(el.size),
			type(el.type),
			attribute(el.attribute),
			buffer(el.buffer),
			normalize(el.normalize)
		{
		}

		VertexArrayElement& VertexArrayElement::operator=(const VertexArrayElement& el)
		{
			size = el.size;
			type = el.type;
			attribute = el.attribute;
			buffer = el.buffer;
			normalize = el.normalize;

			return *this;
		}
	}
}