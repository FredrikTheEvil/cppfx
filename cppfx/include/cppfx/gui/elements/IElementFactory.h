#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_IELEMENTFACTORY_H
#define __CPPFX_GUI_ELEMENTS_IELEMENTFACTORY_H

#include <cppfx/Referenced.h>
#include <cppfx/ref_ptr.h>
#include <cppfx/string.h>
#include <cppfx/gui/elements/Element.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API IElementFactory : public virtual Referenced {
			public:
				virtual ref_ptr<Element> createElement(const string& tag, const ref_ptr<Element>& parent) = 0;
			protected:
				virtual ~IElementFactory();
			};
		}
	}
}

#endif