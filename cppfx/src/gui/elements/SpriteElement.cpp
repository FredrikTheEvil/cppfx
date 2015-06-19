#include <cppfx/gui/elements/SpriteElement.h>
#include <cppfx/gui/elements/Document.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			SpriteElement::SpriteElement(const ref_ptr<Element>& parent) :
				Element(parent)
			{
				sprite = createWidget<SpriteWidget>();
			}
			void SpriteElement::setSprite(const SlicedSprite& spr, const ref_ptr<graphics::Texture2D> texture) {
				sprite->setSprite(spr, texture);
			}
			void SpriteElement::setSpriteColor(const colorf& color) {
				sprite->setColor(color);
			}
			const SlicedSprite& SpriteElement::getSprite() const {
				return sprite->getSprite();
			}
			const colorf& SpriteElement::getSpriteColor() const {
				return sprite->getColor();
			}

			void SpriteElement::setAttribute(const string& key, const string& value) {
				if (key == "sprite") {
					SlicedSprite spr;
					ref_ptr<graphics::Texture2D> tex;
					if (resolveSprite(value, spr, tex)) {
						setSprite(spr, tex);
						spriteName = value;
					}
				}
				else if (key == "sprite-color")
				{
					sprite->setColor(parseColor(value));
				}
				else
					Element::setAttribute(key, value);
			}
			string SpriteElement::getAttribute(const string& key) const {
				if (key == "sprite")
					return spriteName;
				else if (key == "sprite-color")
					sprite->setColor(colors::white);
				else
					return Element::getAttribute(key);
			}
			void SpriteElement::removeAttribute(const string& key) {
				if (key == "sprite") {
					SlicedSprite spr;
					sprite->setSprite(spr, nullptr);
					spriteName = "";
				}
				else if (key == "sprite-color")
					sprite->setColor(colors::white);
				else
					Element::removeAttribute(key);
			}

			bool SpriteElement::hasAttribute(const string& key) const {
				if (key == "sprite")
					return spriteName.size() > 0;
				else if (key == "sprite-color")
					return sprite->getColor() != colors::white;
				else
					return Element::hasAttribute(key);
			}
		}
	}
}