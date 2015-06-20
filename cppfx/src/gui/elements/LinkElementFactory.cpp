#include <cppfx/gui/elements/LinkElementFactory.h>
#include <cppfx/Exceptions.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			LinkElementFactory::LinkElementFactory() {
			}
			LinkElementFactory::~LinkElementFactory() {
			}

			ref_ptr<Element> LinkElementFactory::createElement(const string& tag, const ref_ptr<Element>& parent) {
				if (tag != "link")
					throw RuntimeError("invalid tag");
				return new LinkElement(parent);
			}
		}
	}
}