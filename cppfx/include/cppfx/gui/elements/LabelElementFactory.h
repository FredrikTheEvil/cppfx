#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_LABELELEMENTFACTORY_H
#define __CPPFX_GUI_ELEMENTS_LABELELEMENTFACTORY_H

#include <cppfx/gui/elements/LabelElement.h>
#include <cppfx/gui/elements/IElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API LabelElementFactory : public Object, public virtual IElementFactory {
			public:
				LabelElementFactory();
				ref_ptr<Element> createElement(const string& tagName, const ref_ptr<Element>& parent);
			private:
				~LabelElementFactory();
			};
		}
	}
}

#endif