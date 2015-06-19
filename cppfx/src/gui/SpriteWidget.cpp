#include <cppfx/gui/SpriteWidget.h>

namespace cppfx
{
	namespace gui
	{
		SpriteWidget::SpriteWidget(const ref_ptr<WidgetManager> manager) :
			Widget(manager),
			color(colors::white)
		{
		}

		void SpriteWidget::setSprite(const SlicedSprite& sprite, const ref_ptr<graphics::Texture2D> texture)
		{
			if (SpriteWidget::sprite != sprite)
			{
				SpriteWidget::sprite = sprite;
				dirty = true;
			}
			if (SpriteWidget::texture != texture)
			{
				SpriteWidget::texture = texture;
				dirty = true;
				manager->needRedrawIndices = true;
			}
			
		}

		void SpriteWidget::setColor(const colorf& color)
		{
			if (SpriteWidget::color != color)
			{
				SpriteWidget::color = color;
				dirty = true;
			}
		}

		const colorf& SpriteWidget::getColor() const
		{
			return color;
		}

		const SlicedSprite& SpriteWidget::getSprite() const
		{
			return sprite;
		}
		const ref_ptr<graphics::Texture2D>& SpriteWidget::getTexture()
		{
			return texture;
		}

		inline void drawBatchSprite(
			const colorf& color, 
			const vector2i& textureSize,
			const vector4i& sprite,
			const vector4i& rect,

			SpriteBatchSprite& s)
		{
			s.topLeft.uv =
				vector2f(float(sprite.x) / float(textureSize.x), float(sprite.y) / float(textureSize.y));
			s.topLeft.pos =
				vector2f(rect.x, rect.y);
			s.topLeft.color = color;

			s.topRight.uv =
				vector2f(float(sprite.x + sprite.z) / float(textureSize.x), float(sprite.y) / float(textureSize.y));
			s.topRight.pos =
				vector2f(rect.x + rect.z, rect.y);
			s.topRight.color = color;

			s.bottomLeft.uv =
				vector2f(float(sprite.x) / float(textureSize.x), float(sprite.y + sprite.w) / float(textureSize.y));
			s.bottomLeft.pos =
				vector2f(rect.x, rect.y + rect.w);
			s.bottomLeft.color = color;

			s.bottomRight.uv =
				vector2f(float(sprite.x + sprite.z) / float(textureSize.x), float(sprite.y + sprite.w) / float(textureSize.y));
			s.bottomRight.pos =
				vector2f(rect.x + rect.z, rect.y + rect.w);
			s.bottomRight.color = color;
		}


		void SpriteWidget::redraw()
		{
			clear();

			if (!texture.valid()) {
				dirty = false;
				return;
			}

			SpriteBatchSprite s;
			vector4i rect;
			rect.x = area.x + margin.x;
			rect.y = area.y + margin.y;
			rect.z = area.z - (margin.x + margin.z);
			rect.w = area.w - (margin.y + margin.w);

			if (rect.z <= 0 || rect.w <= 0)
				return;

			int nWayScaling = 0;
			vector2i texSize = vector2i(texture->getWidth(), texture->getHeight());

			if (rect.z > sprite.rect.z && (sprite.edges.x != 0 || sprite.edges.z != 0))
			{
				// Use 3 way horizontal scaling
				nWayScaling |= 1;
			}
			if (rect.w > sprite.rect.w && (sprite.edges.y != 0 || sprite.edges.w != 0))
			{
				// Use 3 way vertical scaling
				nWayScaling |= 2;

			}
			SpriteBatchSprite batchSprite;

			if ((nWayScaling & 3) == 3) {
				// Need 9-way scaling
				vector4i topLeft(sprite.rect.x, sprite.rect.y, sprite.edges.x, sprite.edges.y);
				vector4i topRight(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y, sprite.edges.z, sprite.edges.y);
				vector4i bottomLeft(sprite.rect.x, sprite.rect.y + sprite.rect.w - sprite.edges.w, sprite.edges.x, sprite.edges.w);
				vector4i bottomRight(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y + sprite.rect.w - sprite.edges.w, sprite.edges.z, sprite.edges.w);

				vector4i left(sprite.rect.x, sprite.rect.y + sprite.edges.y, sprite.edges.x, sprite.rect.z - (sprite.edges.x + sprite.edges.z));
				vector4i right(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y + sprite.edges.y, sprite.edges.z, sprite.rect.z - (sprite.edges.x + sprite.edges.z));
				vector4i top(sprite.rect.x + sprite.edges.x, sprite.rect.y, sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.edges.y);
				vector4i bottom(sprite.rect.x + sprite.edges.x, (sprite.rect.y + sprite.rect.w) - (sprite.edges.w), sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.edges.w);

				vector4i center(sprite.rect.x + sprite.edges.x, sprite.rect.y + sprite.edges.y, sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.rect.w - (sprite.edges.y + sprite.edges.w));

				int width = rect.z - (sprite.edges.x + sprite.edges.z);
				int height = rect.w - (sprite.edges.y + sprite.edges.w);

				vector4i centerR(rect.x + sprite.edges.x, rect.y + sprite.edges.y, width, height);

				vector4i leftR(rect.x, rect.y + sprite.edges.y, sprite.edges.x, height);
				vector4i rightR(rect.x + sprite.edges.x + width, rect.y + sprite.edges.y, sprite.edges.z, height);
				vector4i topR(rect.x + sprite.edges.x, rect.y, width, sprite.edges.y);
				vector4i bottomR(rect.x + sprite.edges.x, rect.y + sprite.edges.y + height, width, sprite.edges.w);

				vector4i topLeftR(rect.x, rect.y, sprite.edges.x, sprite.edges.y);
				vector4i topRightR(rect.x + sprite.edges.x + width, rect.y, sprite.edges.z, sprite.edges.y);
				vector4i bottomLeftR(rect.x, rect.y + sprite.edges.y + height, sprite.edges.x, sprite.edges.w);
				vector4i bottomRightR(rect.x + sprite.edges.x + width, rect.y + sprite.edges.y + height, sprite.edges.z, sprite.edges.w);

				drawBatchSprite(color, texSize, center, centerR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, left, leftR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, right, rightR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, bottom, bottomR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, top, topR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, topLeft, topLeftR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, topRight, topRightR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, bottomLeft, bottomLeftR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, bottomRight, bottomRightR, batchSprite);
				addSprite(batchSprite, texture);
			}
			else if ((nWayScaling & 1) == 1)
			{
				// Need 3-way scaling horizontally
				vector4i left(sprite.rect.x, sprite.rect.y, sprite.edges.x, sprite.rect.w);
				vector4i center(sprite.rect.x + sprite.edges.x, sprite.rect.y, sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.rect.w);
				vector4i right(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y, sprite.edges.z, sprite.rect.w);

				int len = rect.z - (sprite.edges.x + sprite.edges.z);
				vector4i leftR = vector4i(rect.x, rect.y, sprite.edges.x, rect.w);
				vector4i centerR = vector4i(rect.x + sprite.edges.x, rect.y, len, rect.w);
				vector4i rightR = vector4i(rect.x + sprite.edges.x + len, rect.y, sprite.edges.z, rect.w);
				
				drawBatchSprite(color, texSize, left, leftR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, center, centerR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, right, rightR, batchSprite);
				addSprite(batchSprite, texture);
			}
			else if ((nWayScaling & 2) == 2)
			{
				// Ned 3-way scaling vertically
				vector4i top(sprite.rect.x, sprite.rect.y, sprite.rect.z, sprite.edges.y);
				vector4i center(sprite.rect.x, sprite.rect.y + sprite.edges.y, sprite.rect.z, sprite.rect.w - (sprite.edges.y + sprite.edges.w));
				vector4i bottom(sprite.rect.x, sprite.rect.y + sprite.rect.w - sprite.edges.w, sprite.rect.z, sprite.edges.w);

				int len = rect.w - (sprite.edges.y + sprite.edges.w);
				vector4i topR = vector4i(rect.x, rect.y, rect.z, sprite.edges.y);
				vector4i centerR = vector4i(rect.x, rect.y + sprite.edges.y, rect.z, len);
				vector4i bottomR = vector4i(rect.x, rect.y + sprite.edges.y + len, rect.z, sprite.edges.w);
				drawBatchSprite(color, texSize, top, topR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, center, centerR, batchSprite);
				addSprite(batchSprite, texture);
				drawBatchSprite(color, texSize, bottom, bottomR, batchSprite);
				addSprite(batchSprite, texture);
			}
			else
			{
				// Use 1-way scaling
				drawBatchSprite(color, texSize, sprite.rect, rect, s);
				addSprite(batchSprite, texture);
			}
		}
		void SpriteWidget::render()
		{
			if (dirty) {
				SpriteBatchSprite s;
				vector4i rect;
				rect.x = area.x + margin.x;
				rect.y = area.y + margin.y;
				rect.z = area.z - (margin.x + margin.z);
				rect.w = area.w - (margin.y + margin.w);

				if (rect.z <= 0 || rect.w <= 0)
				{
					clear();
					return;	
				}

				int nWayScaling = 0;
				vector2i texSize = vector2i(texture->getWidth(), texture->getHeight());

				if (rect.z > sprite.rect.z && (sprite.edges.x != 0 || sprite.edges.z != 0))
				{
					// Use 3 way horizontal scaling
					nWayScaling |= 1;
				}
				if (rect.w > sprite.rect.w && (sprite.edges.y != 0 || sprite.edges.w != 0))
				{
					// Use 3 way vertical scaling
					nWayScaling |= 2;
				}
				SpriteBatchSprite batchSprite;

				if ((nWayScaling & 3) == 3) {
					if (sprites.size() != 9)
					{
						manager->needRedrawIndices = true;
						return;
					}

					// Need 9-way scaling
					vector4i topLeft(sprite.rect.x, sprite.rect.y, sprite.edges.x, sprite.edges.y);
					vector4i topRight(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y, sprite.edges.z, sprite.edges.y);
					vector4i bottomLeft(sprite.rect.x, sprite.rect.y + sprite.rect.w - sprite.edges.w, sprite.edges.x, sprite.edges.w);
					vector4i bottomRight(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y + sprite.rect.w - sprite.edges.w, sprite.edges.z, sprite.edges.w);

					vector4i left(sprite.rect.x, sprite.rect.y + sprite.edges.y, sprite.edges.x, sprite.rect.z - (sprite.edges.x + sprite.edges.z));
					vector4i right(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y + sprite.edges.y, sprite.edges.z, sprite.rect.z - (sprite.edges.x + sprite.edges.z));
					vector4i top(sprite.rect.x + sprite.edges.x, sprite.rect.y, sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.edges.y);
					vector4i bottom(sprite.rect.x + sprite.edges.x, (sprite.rect.y + sprite.rect.w) - (sprite.edges.w), sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.edges.w);

					vector4i center(sprite.rect.x + sprite.edges.x, sprite.rect.y + sprite.edges.y, sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.rect.w - (sprite.edges.y + sprite.edges.w));

					int width = rect.z - (sprite.edges.x + sprite.edges.z);
					int height = rect.w - (sprite.edges.y + sprite.edges.w);

					vector4i centerR(rect.x + sprite.edges.x, rect.y + sprite.edges.y, width, height);

					vector4i leftR(rect.x, rect.y + sprite.edges.y, sprite.edges.x, height);
					vector4i rightR(rect.x + sprite.edges.x + width, rect.y + sprite.edges.y, sprite.edges.z, height);
					vector4i topR(rect.x + sprite.edges.x, rect.y, width, sprite.edges.y);
					vector4i bottomR(rect.x + sprite.edges.x, rect.y + sprite.edges.y + height, width, sprite.edges.w);

					vector4i topLeftR(rect.x, rect.y, sprite.edges.x, sprite.edges.y);
					vector4i topRightR(rect.x + sprite.edges.x + width, rect.y, sprite.edges.z, sprite.edges.y);
					vector4i bottomLeftR(rect.x, rect.y + sprite.edges.y + height, sprite.edges.x, sprite.edges.w);
					vector4i bottomRightR(rect.x + sprite.edges.x + width, rect.y + sprite.edges.y + height, sprite.edges.z, sprite.edges.w);

					drawBatchSprite(color, texSize, center, centerR, batchSprite);
					updateSprite(0, batchSprite, texture);
					drawBatchSprite(color, texSize, left, leftR, batchSprite);
					updateSprite(1, batchSprite, texture);
					drawBatchSprite(color, texSize, right, rightR, batchSprite);
					updateSprite(2, batchSprite, texture);
					drawBatchSprite(color, texSize, bottom, bottomR, batchSprite);
					updateSprite(3, batchSprite, texture);
					drawBatchSprite(color, texSize, top, topR, batchSprite);
					updateSprite(4, batchSprite, texture);
					drawBatchSprite(color, texSize, topLeft, topLeftR, batchSprite);
					updateSprite(5, batchSprite, texture);
					drawBatchSprite(color, texSize, topRight, topRightR, batchSprite);
					updateSprite(6, batchSprite, texture);
					drawBatchSprite(color, texSize, bottomLeft, bottomLeftR, batchSprite);
					updateSprite(7, batchSprite, texture);
					drawBatchSprite(color, texSize, bottomRight, bottomRightR, batchSprite);
					updateSprite(8, batchSprite, texture);
				}
				else if ((nWayScaling & 1) == 1)
				{
					if (sprites.size() != 3)
					{
						manager->needRedrawIndices = true;
						return;
					}

					// Need 3-way scaling horizontally
					vector4i left(sprite.rect.x, sprite.rect.y, sprite.edges.x, sprite.rect.w);
					vector4i center(sprite.rect.x + sprite.edges.x, sprite.rect.y, sprite.rect.z - (sprite.edges.x + sprite.edges.z), sprite.rect.w);
					vector4i right(sprite.rect.x + sprite.rect.z - sprite.edges.z, sprite.rect.y, sprite.edges.z, sprite.rect.w);

					int len = rect.z - (sprite.edges.x + sprite.edges.z);
					vector4i leftR = vector4i(rect.x, rect.y, sprite.edges.x, rect.w);
					vector4i centerR = vector4i(rect.x + sprite.edges.x, rect.y, len, rect.w);
					vector4i rightR = vector4i(rect.x + sprite.edges.x + len, rect.y, sprite.edges.z, rect.w);

					drawBatchSprite(color, texSize, left, leftR, batchSprite);
					updateSprite(0, batchSprite, texture);
					drawBatchSprite(color, texSize, center, centerR, batchSprite);
					updateSprite(1, batchSprite, texture);
					drawBatchSprite(color, texSize, right, rightR, batchSprite);
					updateSprite(2, batchSprite, texture);
				}
				else if ((nWayScaling & 2) == 2)
				{
					if (sprites.size() != 3)
					{
						manager->needRedrawIndices = true;
						return;
					}
					// Ned 3-way scaling vertically
					vector4i top(sprite.rect.x, sprite.rect.y, sprite.rect.z, sprite.edges.y);
					vector4i center(sprite.rect.x, sprite.rect.y + sprite.edges.y, sprite.rect.z, sprite.rect.w - (sprite.edges.y + sprite.edges.w));
					vector4i bottom(sprite.rect.x, sprite.rect.y + sprite.rect.w - sprite.edges.w, sprite.rect.z, sprite.edges.w);

					int len = rect.w - (sprite.edges.y + sprite.edges.w);
					vector4i topR = vector4i(rect.x, rect.y, rect.z, sprite.edges.y);
					vector4i centerR = vector4i(rect.x, rect.y + sprite.edges.y, rect.z, len);
					vector4i bottomR = vector4i(rect.x, rect.y + sprite.edges.y + len, rect.z, sprite.edges.w);
					drawBatchSprite(color, texSize, top, topR, batchSprite);
					updateSprite(0, batchSprite, texture);
					drawBatchSprite(color, texSize, center, centerR, batchSprite);
					updateSprite(1, batchSprite, texture);
					drawBatchSprite(color, texSize, bottom, bottomR, batchSprite);
					updateSprite(2, batchSprite, texture);
				}
				else
				{
					if (sprites.size() != 1)
					{
						manager->needRedrawIndices = true;
						return;
					}
					// Use 1-way scaling
					drawBatchSprite(color, texSize, sprite.rect, rect, s);
					updateSprite(0, batchSprite, texture);
				}
				dirty = false;
			}
		}
	}
}