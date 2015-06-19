#pragma once
#ifndef __CPPFX_GUI_SPRITEWIDGET_H
#define __CPPFX_GUI_SPRITEWIDGET_H
#include <cppfx/gui/SpriteSheet.h>
#include <cppfx/gui/elements/Element.h>

namespace cppfx
{
	namespace gui
	{
		class CPPFX_API SpriteWidget : public Widget
		{
		public:
			SpriteWidget(const ref_ptr<WidgetManager> manager);

			void setSprite(const SlicedSprite& sprite, const ref_ptr<graphics::Texture2D> texture);
			void setColor(const colorf& color);
			const colorf& getColor() const;
			const SlicedSprite& getSprite() const;
			const ref_ptr<graphics::Texture2D>& getTexture();
		protected:
			virtual void redraw() override;
			virtual void render() override;
		private:
			SlicedSprite sprite;
			colorf color;
			ref_ptr<graphics::Texture2D> texture;
		};
	}
}

#endif