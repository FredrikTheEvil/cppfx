#include <cppfx/gui/elements/LabelElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			LabelElementFactory::LabelElementFactory() {
			}
			LabelElementFactory::~LabelElementFactory() {
			}
			ref_ptr<Element> LabelElementFactory::createElement(const string& tagName, const ref_ptr<Element>& parent) {
				return new LabelElement(parent);
			}
		}
	}
}