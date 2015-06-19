#include <cppfx/gui/elements/BodyElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			BodyElementFactory::BodyElementFactory() {
			}
			ref_ptr<Element> BodyElementFactory::createElement(const string& tag, const ref_ptr<Element>& parent) {
				return new BodyElement(parent);
			}
			BodyElementFactory::~BodyElementFactory() {
			}
		}
	}
}