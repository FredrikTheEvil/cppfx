#include <cppfx/gui/elements/BodyElement.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			BodyElement::BodyElement(const ref_ptr<Element>& element) :
				Element(element)
			{
				setRect(vector4f(0.0f, 0.0f, 1.0f, 1.0f));
			}
		}
	}
}