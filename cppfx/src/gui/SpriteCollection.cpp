#include <cppfx/gui/SpriteCollection.h>

namespace cppfx {
	namespace gui {
		SpriteCollection::SpriteCollection(const ref_ptr<graphics::Context>& context, const string& basePath) :
			context(context),
			basePath(basePath)
		{ 
		}
		ref_ptr<SpriteSheet> SpriteCollection::loadSpriteSheet(const string& href) {
			auto i = spriteSheets.find(href);
			if (i != spriteSheets.end()) {
				return i->second;
			}
			ref_ptr<SpriteSheet> sheet = SpriteSheet::loadSpriteSheet(context, basePath + href);
			spriteSheets.insert(std::pair<string, ref_ptr<SpriteSheet> >(href, sheet));
			return sheet;
		}
		ref_ptr<BitmapFont> SpriteCollection::loadBitmapFont(const string& href)
		{
			auto i = fonts.find(href);
			if (i != fonts.end())
				return i->second;
			ref_ptr<BitmapFont> font = BitmapFont::loadBitmapFont(this, basePath + href);
			if (font.valid())
			{
				addFont(href, font);
			}
			return font;
		}
		void SpriteCollection::addSpriteSheet(const string& key, const ref_ptr<SpriteSheet>& spriteSheet) {
			spriteSheets[key] = spriteSheet;
		}
		void SpriteCollection::addFont(const string& key, const ref_ptr<BitmapFont>& font) {
			fonts[key] = font;
		}
		bool SpriteCollection::tryGetSprite(const string& key, SlicedSprite& sprite, ref_ptr<graphics::Texture2D>& texture)
		{
			for (auto itr = spriteSheets.begin(); itr != spriteSheets.end(); ++itr) {
				if (itr->second->getSlicedSprite(key, sprite)) {
					texture = itr->second->getTexture();
					if (texture.valid())
						return true;
					else
						texture = nullptr;
				}
			}
			if (cachedTextures.tryGet(key, texture)) {
				sprite.edges = vector4f();
				sprite.rect = vector4f(0.0f, 0.0f, texture->getWidth(), texture->getHeight());
				return true;
			}
			if (spriteFileMisses.find(key) != spriteFileMisses.end())
			{
				texture = nullptr;
				return false;
			}
			ref_ptr<graphics::Texture2D> tex = context->loadTexture2D(basePath + key);
			if (tex.valid()) {
				cachedTextures.insert(key, texture);
				texture = tex;
				sprite.edges = vector4f();
				sprite.rect = vector4f(0.0f, 0.0f, texture->getWidth(), texture->getHeight());
				return true;
			}
			spriteFileMisses.insert(key);
			return false;
		}
		const ref_ptr<graphics::Context>& SpriteCollection::getGraphicsContext() {
			return context;
		}
	}
}