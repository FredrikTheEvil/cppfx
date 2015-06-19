#include <cppfx/gui/LabelWidget.h>

namespace cppfx
{
	namespace gui
	{
		LabelWidget::LabelWidget(const ref_ptr<WidgetManager> manager) :
			Widget(manager),
			color(colors::white),
			primaryColor(colors::white),
			secondaryColor(colors::white),
			font(nullptr),
			text(""),
			useWaveY(false),
			useWaveColor(false),
			waveY(0.0f),
			waveAmount(3.0f),
			waveSpeed(3.5f),
			waveTimeY(0.0f),
			waveTimeColor(0.0f)
		{
		}

		void LabelWidget::setWaveEffectEnabled(bool enabled)
		{
			useWaveY = enabled;
		}

		bool LabelWidget::getWaveEffectEnabled() const
		{
			return useWaveY;
		}

		void LabelWidget::setWaveAmount(float x) {
			waveAmount = x;
		}
		void LabelWidget::setWaveSpeed(float x) {
			waveSpeed = x;
		}
		float LabelWidget::getWaveAmount() const {
			return waveAmount;
		}
		float LabelWidget::getWaveSpeed() const {
			return waveSpeed;
		}

		void LabelWidget::setWaveColorEffectEnabled(bool enabled)
		{
			if (enabled != useWaveColor)
			{
				useWaveColor = true;
			}
		}

		bool LabelWidget::getWaveColorEffectEnabled() const
		{
			return useWaveColor;
		}

		void LabelWidget::setFont(const ref_ptr<BitmapFont>& font)
		{
			if (LabelWidget::font != font)
			{
				LabelWidget::font = font;
				dirty = true;
			}
		}

		void LabelWidget::setColor(const colorf& color)
		{
			if (LabelWidget::color != color)
			{
				LabelWidget::color = color;
				dirty = true;
			}
			primaryColor = color;
		}

		void LabelWidget::setSecondaryColor(const colorf& color)
		{
			if (secondaryColor != color)
			{
				secondaryColor = color;
				dirty = true;
			}
		}

		void LabelWidget::setText(const string& text)
		{
			if (LabelWidget::text != text)
			{
				LabelWidget::text = text;
				dirty = true;
			}
		}

		const ref_ptr<BitmapFont>& LabelWidget::getFont()
		{
			return font;
		}

		const colorf& LabelWidget::getColor() const
		{
			return color;
		}

		const colorf& LabelWidget::getSecondaryColor() const
		{
			return secondaryColor;
		}

		const string& LabelWidget::getText() const
		{
			return text;
		}

		void LabelWidget::update(double deltaTime)
		{
			if (useWaveY)
			{
				waveTimeY += deltaTime;
				float sinWaveY = sin(waveTimeY * waveSpeed);
				float sinWaveScaledZeroToOneY = (sinWaveY + 1.0f) * 0.5f;
				float sinWaveScaledFromPointFiveToOneY = sinWaveScaledZeroToOneY * 0.5f + 0.5f;
				waveTimeY += deltaTime;
				float amount = sinWaveY * waveAmount;
				if (waveY != amount)
				{
					waveY = amount;
					dirty = true;
				}

			}
			else
			{
				if (waveY != 0)
				{
					waveTimeY = 0.0f;
					waveY = 0;
					dirty = true;
				}
			}
			if (useWaveColor)
			{
				waveTimeColor += deltaTime;
				float sinWaveColor = sin(waveTimeColor * waveSpeed);
				float sinWaveScaledZeroToOneColor = (sinWaveColor + 1.0f) * 0.5f;
				float sinWaveScaledFromPointFiveToOneColor = sinWaveScaledZeroToOneColor * 0.5f + 0.5f;
				colorf clr = lerp(primaryColor, secondaryColor, sinWaveScaledZeroToOneColor);
				if (clr != color)
				{
					color = clr;
					dirty = true;
				}
			} 
			else
			{
				if (color != primaryColor)
				{
					waveTimeColor = 0.0f;
					color = primaryColor;
					dirty = true;
				}
			}

			Widget::update(deltaTime);
		}

		void LabelWidget::redraw()
		{
			clear();
			if (!font.valid())
				return;
			vector4i rect;
			rect.x = area.x + margin.x;
			rect.y = area.y + margin.y + waveY;
			rect.z = area.z - (margin.x + margin.z);
			rect.w = area.w - (margin.y + margin.w);

			if (rect.z <= 0 || rect.w <= 0)
				return;
			
			vector2f pos = vector2i(rect.x, rect.y);

			int numPages = font->pages.size();
			SpriteBatchSprite s;
			for (auto chPtr = text.begin(); chPtr != text.end(); ++chPtr)
			{
				char32_t ch(*chPtr);
				const BitmapFontGlyph& glyph = font->getGlyph(ch);
				if (glyph.page < 0 || glyph.page >= numPages)
					throw std::runtime_error("page index out of range");
				const BitmapFontPage& page = font->pages[glyph.page];
				font->emit(s, rect, pos, color, glyph, page);
				addSprite(s, page.texture);
			}
		}

		void LabelWidget::render()
		{
			if (dirty) {
				if (text.size() != sprites.size())
				{
					manager->needRedrawIndices = true;
					return;
				}

				vector4i rect;
				rect.x = area.x + margin.x;
				rect.y = area.y + margin.y + waveY;
				rect.z = area.z - (margin.x + margin.z);
				rect.w = area.w - (margin.y + margin.w);

				if (rect.z <= 0 || rect.w <= 0)
					return;

				vector2f pos = vector2i(rect.x, rect.y);

				if (!font.valid())
					return;

				int numPages = font->pages.size();
				SpriteBatchSprite s;
				int i = 0;
				for (auto chPtr = text.begin(); chPtr != text.end(); ++chPtr)
				{
					if (i >= sprites.size())
					{
						manager->needRedrawIndices = true;
						return;
					}

					char32_t ch(*chPtr);
					const BitmapFontGlyph& glyph = font->getGlyph(ch);
					if (glyph.page < 0 || glyph.page >= numPages)
						throw std::runtime_error("page index out of range");
					const BitmapFontPage& page = font->pages[glyph.page];
					font->emit(s, rect, pos, color, glyph, page);
					updateSprite(i, s, page.texture);
					i++;
				}
				dirty = false; 

			}
		}
	}
}