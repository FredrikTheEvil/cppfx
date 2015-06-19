#include <cppfx/gui/elements/SpriteElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			SpriteElementFactory::SpriteElementFactory() {

			}
			SpriteElementFactory::~SpriteElementFactory() {

			}
			ref_ptr<Element> SpriteElementFactory::createElement(const string& tagName, const ref_ptr<Element>& parent) {
				return new SpriteElement(parent);
			}
		}
	}
}