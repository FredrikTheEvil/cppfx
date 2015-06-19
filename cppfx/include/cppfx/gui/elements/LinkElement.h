#pragma once
#ifndef __CPPFX_GUI_ELEMENT_LINKELEMENT_H
#define __CPPFX_GUI_ELEMENT_LINKELEMENT_H

#include <cppfx/gui/elements/Element.h>
#include <cppfx/gui/SpriteWidget.h>
#include <cppfx/gui/LabelWidget.h>
#include <cppfx/gui/BitmapFont.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			class CPPFX_API LinkElement : public Element {
			public:
				explicit LinkElement(const ref_ptr<Element>& parent);
				void setHref(const string& href);
				void setRel(const string& rel);
				const string& getHref() const;
				const string& getRel() const;

				virtual void setAttribute(const string& key, const string& value) override;
				virtual string getAttribute(const string& key) const override;
				virtual void removeAttribute(const string& key) override;
				virtual bool hasAttribute(const string& key) const override;
			private:
				void activate();
				void activateSpriteSheet(Document* doc);
				void activateFont(Document* doc);

				enum LinkState {
					INACTIVE,
					FAILED,
					LINKED
				};
				LinkState state;
				string rel;
				string href;
				ref_ptr<Object> target;
			};
		}
	}
}

#endif