#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_BUTTONELEMENTFACTORY_H
#define __CPPFX_GUI_ELEMENTS_BUTTONELEMENTFACTORY_H

#include <cppfx/gui/elements/IElementFactory.h>
#include <cppfx/gui/elements/ButtonElement.h>
#include <cppfx/Object.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API ButtonElementFactory : public Object, public virtual IElementFactory {
			public:
				ButtonElementFactory();
				virtual ref_ptr<Element> createElement(const string& tag, const ref_ptr<Element>& parent) override;
			protected:
				virtual ~ButtonElementFactory();
			};
		}
	}
}

#endif