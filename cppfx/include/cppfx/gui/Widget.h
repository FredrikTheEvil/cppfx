#pragma once
#ifndef __CPPFX_GUI_WIDGET_H
#define __CPPFX_GUI_WIDGET_H

#include <cppfx/gui/WidgetManager.h>

namespace cppfx
{
	namespace gui
	{
		struct CPPFX_API WidgetSprite
		{
			SpriteBatchSprite batchSprite;
			ref_ptr<graphics::Texture2D> texture;
		};
		class CPPFX_API Widget : public Object
		{
		public:
			void setArea(const vector4f& area);
			void setMargin(const vector4f& margin);
			const vector4f& getArea() const;
			const vector4f& getMargin() const;
			virtual void update(double time);
		protected:
			Widget(const ref_ptr<WidgetManager>& manager);
			virtual ~Widget();

			void clear();
			unsigned int addSprite(const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D> texture);
			const SpriteBatchSprite& getSprite(unsigned int i);
			unsigned int getNumSprites() const;
			void updateSprite(unsigned int i, const SpriteBatchSprite& sprite);
			void updateSprite(unsigned int i, const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D> texture);
			
			virtual void redraw() = 0;
			/// <param name="rect">Rect of containing element</param>
			virtual void render() = 0;

			bool dirty;
			vector4f area;
			vector4f margin;
			int zIndex;
			ref_ptr<WidgetManager> manager;
			std::vector<WidgetSprite> sprites;
			bool verticesDirty;
			int vertexOffset; // Offset in sprite batch of first vertex
		private:
			

			friend class WidgetManager;
		};
	}
}

#endif