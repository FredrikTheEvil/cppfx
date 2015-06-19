#include <cppfx/gui/elements/LinkElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			LinkElementFactory::LinkElementFactory() {
			}
			LinkElementFactory::~LinkElementFactory() {
			}

			ref_ptr<Element> LinkElementFactory::createElement(const string& tag, const ref_ptr<Element>& parent) {
				if (tag != "link")
					throw std::runtime_error("invalid tag");
				return new LinkElement(parent);
			}
		}
	}
}