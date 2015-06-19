#pragma once
#ifndef __CPPFX_GUI_WIDGETMANAGER_H
#define __CPPFX_GUI_WIDGETMANAGER_H

#include <cppfx/gui/SpriteBatch.h>

namespace cppfx
{
	namespace gui
	{
		namespace elements
		{
			class CPPFX_API Document;
		}
		class CPPFX_API Widget;
		class CPPFX_API LabelWidget;
		class CPPFX_API SpriteWidget;
		
		class CPPFX_API WidgetManager : public Object
		{
		public:
			explicit WidgetManager(const ref_ptr<gui::SpriteBatch>& spriteBatch);

			void render();
			void needRedraw();
		private:
			void redraw();

			ref_ptr<SpriteBatch> spriteBatch;
			std::vector<Widget*> widgets;
			bool needRedrawIndices;

			friend class elements::Document;
			friend class Widget;
			friend class LabelWidget;
			friend class SpriteWidget;
		};
	}
}

#endif