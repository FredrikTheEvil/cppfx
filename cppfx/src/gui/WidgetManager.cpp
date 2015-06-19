#include <cppfx/gui/WidgetManager.h>
#include <cppfx/gui/Widget.h>

namespace cppfx
{
	namespace gui
	{

		void WidgetManager::redraw()
		{
			spriteBatch->clear();
			for (auto w = widgets.begin(); w != widgets.end(); ++w)
			{
				auto widget = *w;
				widget->redraw();
				widget->vertexOffset = spriteBatch->getNumVertices();
				for (auto i = 0; i < widget->getNumSprites(); i++)
				{
					const WidgetSprite& widgetSprite = widget->sprites[i];
					spriteBatch->push(widgetSprite.batchSprite, widgetSprite.texture);
				}
				widget->verticesDirty = false;
			}
			needRedrawIndices = false;
		}

		WidgetManager::WidgetManager(const ref_ptr<gui::SpriteBatch>& spriteBatch) :
			spriteBatch(spriteBatch),
			widgets(),
			needRedrawIndices(true)
		{
		}

		/*void WidgetManager::update()
		{
			for (auto widget = widgets.begin(); widget != widgets.end(); ++widget)
				(*widget)->update(time);
		}*/

		void WidgetManager::render()
		{
			if (needRedrawIndices)
				redraw();
			else {
				for (auto w = widgets.begin(); w != widgets.end(); ++w) {
					auto widget = *w;
					widget->render();
					if (needRedrawIndices)
						break;
					if (widget->verticesDirty)
					{
						for (auto i = 0; i < widget->sprites.size(); i++)
							spriteBatch->updateVertices(widget->vertexOffset + i * 4, widget->sprites[i].batchSprite);
					}
				}
				if (needRedrawIndices)
					redraw();
			}
		}

		void WidgetManager::needRedraw()
		{
			needRedrawIndices = true;
		}
	}
}