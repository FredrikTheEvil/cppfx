#include <cppfx/gui/elements/LabelElement.h>

namespace cppfx {
	namespace gui {
		namespace elements {
			LabelElement::LabelElement(const ref_ptr<Element>& parent) :
				Element(parent)
			{
				label = createWidget<LabelWidget>();
			}
			void LabelElement::setFont(const ref_ptr<BitmapFont>& font) {
				label->setFont(font);
			}
			const ref_ptr<BitmapFont>& LabelElement::getFont() {
				return label->getFont();
			}
			void LabelElement::setColor(const colorf& color) {
				label->setColor(color);
			}
			void LabelElement::setMargin(const vector4f& margin) {
				label->setMargin(margin);
			}
			const colorf& LabelElement::getColor() const {
				return label->getColor();
			}
			void LabelElement::setWaveEffectEnabled(bool enabled) {
				label->setWaveEffectEnabled(enabled);
			}
			bool LabelElement::getWaveEffectEnabled() const {
				return label->getWaveEffectEnabled();
			}

			void LabelElement::setWaveSpeed(float x) {
				label->setWaveSpeed(x);
			}
			void LabelElement::setWaveAmount(float x) {
				label->setWaveAmount(x);
			}
			float LabelElement::getWaveAmount() const {
				return label->getWaveAmount();
			}
			float LabelElement::getWaveSpeed() const {
				return label->getWaveSpeed();
			}

			void LabelElement::setValue(const string& value) {
				label->setText(value);
			}
			const string& LabelElement::getValue() const {
				return label->getText();
			}

			void LabelElement::setAttribute(const string& key, const string& value) {
				if (key == "font") {
					if (value.size() > 0) {
						setFont(resolveFont(value));
					}
					else
					{
						setFont(nullptr);
					}
				}
				else if (key == "value") {
					setValue(value);
				}
				else if (key == "margin-left") {
					auto t = label->getMargin();
					label->setMargin(vector4f(std::stof(value), t.y, t.z, t.w));
				}
				else if (key == "margin-right") {
					auto t = label->getMargin();
					label->setMargin(vector4f(t.x, t.y, std::stof(value), t.w));
				}
				else if (key == "margin-top") {
					auto t = label->getMargin();
					label->setMargin(vector4f(t.x, std::stof(value), t.z, t.w));
				}
				else if (key == "margin-bottom") {
					auto t = label->getMargin();
					label->setMargin(vector4f(t.x, t.y, t.z, std::stof(value)));
				}
				else if (key == "color")
				{
					setColor(parseColor(value));
				}
				else if (key == "wave-effect")
				{
					setWaveEffectEnabled(true);
				}
				else if (key == "wave-amount")
				{
					setWaveAmount(std::stof(value));
				}
				else if (key == "wave-speed")
				{
					setWaveSpeed(std::stof(value));
				}
				else
					Element::setAttribute(key, value);

			}
			string LabelElement::getAttribute(const string& key) const {
				if (key == "font") {
					auto font = label->getFont();
					if (font.valid())
						return font->getName();
					return "";
				}
				else if (key == "value")
					return label->getText();
				else if (key == "margin-left")
					return std::to_string(label->getMargin().x);
				else if (key == "margin-right")
					return std::to_string(label->getMargin().z);
				else if (key == "margin-top")
					return std::to_string(label->getMargin().y);
				else if (key == "margin-bottom")
					return std::to_string(label->getMargin().w);
				else if (key == "color") {
					return toColorString(label->getColor());
				}
				else if (key == "wave-effect") {
					return label->getWaveEffectEnabled() ? "true" : "";
				}
				else if (key == "wave-ammount")
				{
					return std::to_string(label->getWaveAmount());
				}
				else if (key == "wave-speed")
				{
					return std::to_string(label->getWaveSpeed());
				}
				else
					return Element::getAttribute(key);
			}
			void LabelElement::removeAttribute(const string& key) {
				if (key == "font") {
					label->setFont(nullptr);
				}
				else if (key == "value")
				{
					label->setText("");
				}
				else if (key == "margin-left")
				{
					auto m = label->getMargin();
					label->setMargin(vector4f(0.0f, m.y, m.z, m.w));
				}
				else if (key == "margin-right")
				{
					auto m = label->getMargin();
					label->setMargin(vector4f(m.x, m.y, 0.0f, m.w));
				}
				else if (key == "margin-top")
				{
					auto m = label->getMargin();
					label->setMargin(vector4f(m.x, 0.0f, m.z, m.w));
				}
				else if (key == "margin-bottom")
				{
					auto m = label->getMargin();
					label->setMargin(vector4f(m.x, m.y, m.z, 0.0f));
				}
				else if (key == "color") {
					label->setColor(colors::white);
				}
				else if (key == "wave-effect")
				{
					label->setWaveEffectEnabled(false);
				}
				else if (key == "wave-amount")
				{
					label->setWaveAmount(3.0f);
				}
				else if (key == "wave-speed")
				{
					label->setWaveSpeed(3.5f);
				}
				else
					Element::removeAttribute(key);
			}
			bool LabelElement::hasAttribute(const string& key) const {
				if (key == "font")
					return label->getFont().valid();
				else if (key == "value")
					return label->getText().size() > 0;
				else if (key == "margin-left")
					return label->getMargin().x != 0.0f;
				else if (key == "margin-right")
					return label->getMargin().z != 0.0f;
				else if (key == "margin-top")
					return label->getMargin().w != 0.0f;
				else if (key == "color")
					return label->getColor() != colors::white;
				else if (key == "wave-effect")
					return label->getWaveEffectEnabled();
				else if (key == "wave-amount")
					return label->getWaveAmount() != 3.0f;
				else if (key == "wave-speed")
					return label->getWaveSpeed() != 3.5f;
				else
					return Element::hasAttribute(key);
			}
		}
	}
}