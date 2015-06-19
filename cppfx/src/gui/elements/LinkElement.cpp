#include <cppfx/gui/elements/LinkElement.h>
#include <cppfx/gui/elements/Document.h>
#include <iostream>

namespace cppfx {
	namespace gui {
		namespace elements {
			LinkElement::LinkElement(const ref_ptr<Element>& parent) :
				Element(parent),
				state(LinkState::INACTIVE)
			{
				setRect(vector4f());
			}
			void LinkElement::setHref(const string& href) {
				if (state == LinkState::INACTIVE) {
					LinkElement::href = href;
					if (href.size() > 0 && rel.size() > 0)
						activate();
				}
			}
			void LinkElement::setRel(const string& rel) {
				if (state == LinkState::INACTIVE) {
					LinkElement::rel = rel;
					if (href.size() > 0)
						activate();
				}
			}
			const string& LinkElement::getHref() const {
				return href;
			}
			const string& LinkElement::getRel() const {
				return rel;
			}

			void LinkElement::setAttribute(const string& key, const string& value) {
				if (key == "href")
					setHref(value);
				else if (key == "rel")
					setRel(value);
			}
			string LinkElement::getAttribute(const string& key) const {
				if (key == "href")
					return href;
				else if (key == "rel")
					return rel;
			}
			void LinkElement::removeAttribute(const string& key) {
			}
			bool LinkElement::hasAttribute(const string& key) const {
				if (key == "href")
					return href.size() > 0;
				if (key == "rel")
					return rel.size() > 0;
			}

			void LinkElement::activate() {
				Document* doc = getDocument();
				if (doc != nullptr && state == LinkState::INACTIVE) {
					if (rel == "font")
						activateFont(doc);
					else if (rel == "spritesheet")
						activateSpriteSheet(doc);
				}
			}
			void LinkElement::activateFont(Document* doc) {
				auto font = doc->loadFont(href);
				if (font.valid())
					state = LinkState::LINKED;
				else
					state = LinkState::FAILED;
			}
			void LinkElement::activateSpriteSheet(Document* doc) {
				auto spriteSheet = doc->loadSpriteSheet(href);
				if (spriteSheet.valid())
					state = LinkState::LINKED;
				else
					state = LinkState::FAILED;
			}
		}
	}
}