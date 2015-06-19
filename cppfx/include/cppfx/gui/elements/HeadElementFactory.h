#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_HEADELEMENT_FACTORY_H
#define __CPPFX_GUI_ELEMENTS_HEADELEMENT_FACTORY_H

#include <cppfx/gui/elements/IElementFactory.h>
#include <cppfx/gui/elements/HeadElement.h>
#include <cppfx/Object.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API HeadElementFactory : public Object, public virtual IElementFactory {
			public:
				HeadElementFactory();
				virtual ref_ptr<Element> createElement(const string& tag, const ref_ptr<Element>& parent) override;
			protected:
				virtual ~HeadElementFactory();
			};
		}
	}
}

#endif