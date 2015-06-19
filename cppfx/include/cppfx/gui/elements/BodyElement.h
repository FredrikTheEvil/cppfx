#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_BODYELEMENT_H
#define __CPPFX_GUI_ELEMENTS_BODYELEMENT_H

#include <cppfx/gui/elements/Element.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API BodyElement : public Element {
			public:
				BodyElement(const ref_ptr<Element>& parent);
			};
		}
	}
}

#endif