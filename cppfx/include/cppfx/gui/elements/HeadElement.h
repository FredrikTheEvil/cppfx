#pragma once
#ifndef __CPPFX_GUI_ELEMENT_HEADELEMENT_H
#define __CPPFX_GUI_ELEMENT_HEADELEMENT_H

#include <cppfx/gui/elements/Element.h>

namespace cppfx {
	namespace gui {
		namespace elements{
			class CPPFX_API HeadElement : public Element {
			public:
				HeadElement(const ref_ptr<Element>& parent);
			private:
			};
		}
	}
}

#endif