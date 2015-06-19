#include <cppfx/gui/Widget.h>

namespace cppfx
{
	namespace gui
	{
		Widget::Widget(const ref_ptr<WidgetManager>& manager) :
			manager(manager)
		{
			manager->widgets.push_back(this);
		}

		Widget::~Widget()
		{
			if (manager.valid()) {
				for (auto itr = manager->widgets.begin(); itr != manager->widgets.end(); ++itr)
				{
					auto ptr = *itr;
					if (*itr == this)
					{
						manager->widgets.erase(itr);
						manager->needRedrawIndices = true;
						return;
					}
				}
			}
		}

		void Widget::clear()
		{
			if (sprites.size() > 0) {
				sprites.clear();
				manager->needRedrawIndices = true;
			}
		}

		unsigned Widget::addSprite(const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D> texture)
		{
			unsigned int i = sprites.size();
			sprites.push_back({
				sprite,
				texture
			});
			manager->needRedrawIndices = true;
			return i;
		}

		const SpriteBatchSprite& Widget::getSprite(unsigned i)
		{
			if (i >= sprites.size())
				throw std::exception("sprite index out of range");
			return sprites[i].batchSprite;
		}

		unsigned int Widget::getNumSprites() const
		{
			return sprites.size();
		}

		void Widget::updateSprite(unsigned int i, const SpriteBatchSprite& sprite)
		{
			if (i >= sprites.size())
				throw std::exception("sprite index out of range");
			sprites[i].batchSprite = sprite;
			verticesDirty = true;
		}

		void Widget::updateSprite(unsigned i, const SpriteBatchSprite& sprite, const ref_ptr<graphics::Texture2D> texture)
		{
			if (i >= sprites.size())
				throw std::exception("sprite index out of range");
			WidgetSprite &spr = sprites[i];
			spr.batchSprite = sprite;
			if (spr.texture != texture) {
				spr.texture = texture;
				manager->needRedrawIndices = true;
			}
			verticesDirty = true;
		}

		void Widget::setArea(const vector4f& area)
		{
			Widget::area = area;
			dirty = true;
		}

		void Widget::setMargin(const vector4f& margin)
		{
			Widget::margin = margin;
			dirty = true;
		}

		const vector4f& Widget::getArea() const
		{
			return area;
		}

		const vector4f& Widget::getMargin() const
		{
			return margin;
		}

		void Widget::update(double time) // Called to process any updates
		{
		}
	}
}