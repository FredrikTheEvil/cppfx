#include <cppfx/gui/elements/ButtonElementFactory.h>
#include <cppfx/Exceptions.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			ButtonElementFactory::ButtonElementFactory() {
			}
			ButtonElementFactory::~ButtonElementFactory() {
			}

			ref_ptr<Element> ButtonElementFactory::createElement(const string& tag, const ref_ptr<Element>& el) {
				if (tag != "button")
					throw RuntimeError("invalid tag");
				return new ButtonElement(el);
			}
		}
	}
}