#pragma once
#ifndef __CPPFX_GUI_LABELWIDGET_H
#define __CPPFX_GUI_LABELWIDGET_H
#include <cppfx/gui/Widget.h>
#include <cppfx/gui/BitmapFont.h>

namespace cppfx
{
	namespace gui
	{
		class CPPFX_API LabelWidget : public Widget
		{
		public:
			LabelWidget(const ref_ptr<WidgetManager> manager);

			void setWaveEffectEnabled(bool enabled);
			void setWaveAmount(float x);
			void setWaveSpeed(float x);
			bool getWaveEffectEnabled() const;
			void setWaveColorEffectEnabled(bool enabled);
			bool getWaveColorEffectEnabled() const;
			float getWaveAmount() const;
			float getWaveSpeed() const;

			void setFont(const ref_ptr<BitmapFont>& font);
			void setColor(const colorf& color);
			void setSecondaryColor(const colorf& color);
			void setText(const string& text);
			const ref_ptr<BitmapFont>& getFont();
			const colorf& getColor() const;
			const colorf& getSecondaryColor() const;
			const string& getText() const;
			virtual void update(double time) override;
		protected:
			virtual void redraw() override;
			virtual void render() override;
		private:
			colorf color;
			colorf primaryColor;
			colorf secondaryColor;
			ref_ptr<BitmapFont> font;
			string text;
			bool useWaveY;
			bool useWaveColor;
			float waveY;
			float waveAmount;
			float waveSpeed;
			float waveTimeY;
			float waveTimeColor;
		};
	}
}

#endif