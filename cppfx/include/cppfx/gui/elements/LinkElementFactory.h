#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_LINKELEMENTFACTORY_H
#define __CPPFX_GUI_ELEMENTS_LINKELEMENTFACTORY_H

#include <cppfx/gui/elements/IElementFactory.h>
#include <cppfx/gui/elements/LinkElement.h>
#include <cppfx/Object.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API LinkElementFactory : public Object, public virtual IElementFactory {
			public:
				LinkElementFactory();
				virtual ref_ptr<Element> createElement(const string& tag, const ref_ptr<Element>& parent) override;
			protected:
				virtual ~LinkElementFactory();
			};
		}
	}
}

#endif