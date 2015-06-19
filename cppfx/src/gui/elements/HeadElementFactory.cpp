#include <cppfx/gui/elements/HeadElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			HeadElementFactory::HeadElementFactory() {
			}
			ref_ptr<Element> HeadElementFactory::createElement(const string& tag, const ref_ptr<Element>& parent) {
				return new HeadElement(parent);
			}
			HeadElementFactory::~HeadElementFactory() {
			}
		}
	}
}