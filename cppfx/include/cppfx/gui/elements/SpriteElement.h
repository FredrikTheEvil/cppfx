#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_SPRITEELEMENT_H
#define __CPPFX_GUI_ELEMENTS_SPRITEELEMENT_H

#include <cppfx/gui/elements/Element.h>
#include <cppfx/gui/SpriteWidget.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API SpriteElement : public Element {
			public:
				explicit SpriteElement(const ref_ptr<Element>& parent);
				void setSprite(const SlicedSprite& sprite, const ref_ptr<graphics::Texture2D> texture);
				void setSpriteColor(const colorf& color);
				const SlicedSprite& getSprite() const;
				const colorf& getSpriteColor() const;

				virtual void setAttribute(const string& key, const string& value) override;
				virtual string getAttribute(const string& key) const override;
				virtual void removeAttribute(const string& key) override;

				virtual bool hasAttribute(const string& key) const override;
			private:
				ref_ptr<SpriteWidget> sprite;
				string spriteName;
			};
		}
	}
}

#endif