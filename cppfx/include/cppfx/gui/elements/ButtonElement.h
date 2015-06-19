#pragma once
#ifndef __CPPFX_GUI_BUTTONELEMENT_H
#define __CPPFX_GUI_BUTTONELEMENT_H
#include <cppfx/gui/elements/Element.h>
#include <cppfx/gui/SpriteWidget.h>
#include <cppfx/gui/LabelWidget.h>
#include <cppfx/gui/BitmapFont.h>

namespace cppfx
{
	namespace gui
	{
		namespace elements {
			class CPPFX_API ButtonElement : public Element
			{
			public:
				explicit ButtonElement(const ref_ptr<Element>& parent);
				void setText(const string& text);
				const string& getText() const;

				void setSprite(const SlicedSprite& sprite, const ref_ptr<graphics::Texture2D> texture);
				void setSpriteColor(const colorf& color);
				const colorf& getSpriteColor() const;
				void setFont(const ref_ptr<BitmapFont>& font);
				const ref_ptr<BitmapFont>& getFont();
				void setTextColor(const colorf& color);
				void setTextMargin(const vector4f& margin);
				const colorf& getTextColor() const;

				virtual void setAttribute(const string& key, const string& value) override;
				virtual string getAttribute(const string& key) const override;
				virtual void removeAttribute(const string& key) override;
				
				virtual bool hasAttribute(const string& key) const override;
				
			private:
				virtual void reset() override;
				ref_ptr<SpriteWidget> sprite;
				ref_ptr<LabelWidget> label;
			};
		}
	}
}

#endif