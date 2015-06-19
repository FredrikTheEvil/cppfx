#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_BODYELEMENT_FACTORY_H
#define __CPPFX_GUI_ELEMENTS_BODYELEMENT_FACTORY_H

#include <cppfx/gui/elements/IElementFactory.h>
#include <cppfx/gui/elements/BodyElement.h>
#include <cppfx/Object.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API BodyElementFactory : public Object, public virtual IElementFactory {
			public:
				BodyElementFactory();
				virtual ref_ptr<Element> createElement(const string& tag, const ref_ptr<Element>& parent) override;
			protected:
				virtual ~BodyElementFactory();
			};
		}
	}
}

#endif