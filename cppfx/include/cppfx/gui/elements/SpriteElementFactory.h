#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_SPRITEELEMENTFACTORY_H
#define __CPPFX_GUI_ELEMENTS_SPRITEELEMENTFACTORY_H

#include <cppfx/gui/elements/SpriteElement.h>
#include <cppfx/gui/elements/IElementFactory.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API SpriteElementFactory : public Object, public virtual IElementFactory {
			public:
				SpriteElementFactory();
				ref_ptr<Element> createElement(const string& tagName, const ref_ptr<Element>& parent);
			protected:
				~SpriteElementFactory();
			};
		}
	}
}

#endif