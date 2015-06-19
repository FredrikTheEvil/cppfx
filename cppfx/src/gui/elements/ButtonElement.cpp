#include <cppfx/gui/elements/ButtonElement.h>

namespace cppfx
{
	namespace gui
	{
		namespace elements {
			ButtonElement::ButtonElement(const ref_ptr<Element>& parent) :
				Element(parent),
				sprite(),
				label()
			{
				sprite = createWidget<SpriteWidget>();
				label = createWidget<LabelWidget>();
			}

			void ButtonElement::setText(const string& text)
			{
				label->setText(text);
			}

			const string& ButtonElement::getText() const
			{
				return label->getText();
			}

			void ButtonElement::setSprite(const SlicedSprite& sprite, const ref_ptr<graphics::Texture2D> texture)
			{
				ButtonElement::sprite->setSprite(sprite, texture);
			}

			void ButtonElement::setSpriteColor(const colorf& color)
			{
				sprite->setColor(color);
			}

			const colorf& ButtonElement::getSpriteColor() const
			{
				return sprite->getColor();
			}

			void ButtonElement::setFont(const ref_ptr<BitmapFont>& font)
			{
				label->setFont(font);
			}

			const ref_ptr<BitmapFont>& ButtonElement::getFont()
			{
				return label->getFont();
			}

			void ButtonElement::setTextColor(const colorf& color)
			{
				label->setColor(color);
			}

			void ButtonElement::setTextMargin(const vector4f& margin)
			{
				label->setMargin(margin);
			}

			const colorf& ButtonElement::getTextColor() const
			{
				return label->getColor();
			}

			void ButtonElement::reset() {

			}

			void ButtonElement::setAttribute(const string& key, const string& value) {
				if (key == "value") {
					label->setText(value);
				}
				else if (key == "font") {
					auto f = resolveFont(value);
					label->setFont(f);
				}
				else if (key == "text-color") {
					auto color = parseColor(value);
					label->setColor(color);
				}
				else if (key == "text-margin-left")
				{
					vector4f x = label->getMargin();
					x.x = std::stof(value);
					label->setMargin(x);
				}
				else if (key == "text-margin-right")
				{
					vector4f x = label->getMargin();
					x.z = std::stof(value);
					label->setMargin(x);
				}
				else if (key == "text-margin-top")
				{
					vector4f x = label->getMargin();
					x.y = std::stof(value);
					label->setMargin(x);
				}
				else if (key == "text-margin-bottom")
				{
					vector4f x = label->getMargin();
					x.w = std::stof(value);
					label->setMargin(x);
				}
				else if (key == "sprite") {
					SlicedSprite s;
					ref_ptr<graphics::Texture2D> t;
					if (resolveSprite(value, s, t)) {
						sprite->setSprite(s, t);
					}
					else {
						sprite->setSprite(s, nullptr);
					}
				}
				else if (key == "sprite-color") {
					auto color = parseColor(value);
					sprite->setColor(color);
				}
				else if (key == "text-wave-color") {
					label->setWaveColorEffectEnabled(true);
				}
				else if (key == "text-wave-y") {
					label->setWaveEffectEnabled(true);
				}
				else
					Element::setAttribute(key, value);
			}
			void ButtonElement::removeAttribute(const string& key) {
				if (key == "text-wave-y") {
					label->setWaveEffectEnabled(false);
				}
				else
					Element::removeAttribute(key);
			}
			string ButtonElement::getAttribute(const string& key) const {
				return Element::getAttribute(key);
			}
			bool ButtonElement::hasAttribute(const string& key) const {
				if (key == "text-wave-y")
					return label->getWaveEffectEnabled();
				else
					return Element::hasAttribute(key);
			}
		}
	}
}