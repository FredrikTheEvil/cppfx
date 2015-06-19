#include <cppfx/gui/elements/HeadElement.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			HeadElement::HeadElement(const ref_ptr<Element>& parent) :
				Element(parent)
			{
				setRect(vector4f());
			}
		}
	}
}