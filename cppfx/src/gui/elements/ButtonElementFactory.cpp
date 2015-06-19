#include <cppfx/gui/elements/ButtonElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			ButtonElementFactory::ButtonElementFactory() {
			}
			ButtonElementFactory::~ButtonElementFactory() {
			}

			ref_ptr<Element> ButtonElementFactory::createElement(const string& tag, const ref_ptr<Element>& el) {
				if (tag != "button")
					throw std::runtime_error("invalid tag");
				return new ButtonElement(el);
			}
		}
	}
}