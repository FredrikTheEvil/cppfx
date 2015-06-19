#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_LABELELEMENT_H
#define __CPPFX_GUI_ELEMENTS_LABELELEMENT_H

#include <cppfx/gui/elements/Element.h>
#include <cppfx/gui/LabelWidget.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API LabelElement : public Element {
			public:
				LabelElement(const ref_ptr<Element>& parent);
				void setFont(const ref_ptr<BitmapFont>& font);
				const ref_ptr<BitmapFont>& getFont();
				void setColor(const colorf& color);
				void setMargin(const vector4f& margin);
				const colorf& getColor() const;
				void setWaveEffectEnabled(bool enabled);
				bool getWaveEffectEnabled() const;
				void setWaveAmount(float x);
				float getWaveAmount() const;
				void setWaveSpeed(float x);
				float getWaveSpeed() const;

				void setValue(const string& value);
				const string& getValue() const;

				virtual void setAttribute(const string& key, const string& value) override;
				virtual string getAttribute(const string& key) const override;
				virtual void removeAttribute(const string& key) override;

				virtual bool hasAttribute(const string& key) const override;
			private:
				ref_ptr<LabelWidget> label;
			};
		}
	}
}

#endif