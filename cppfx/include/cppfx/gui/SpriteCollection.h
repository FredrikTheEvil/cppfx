#pragma once
#ifndef __CPPFX_GUI_SPRITECOLLECTION_H
#define __CPPFX_GUI_SPRITECOLLECTION_H

#include <cppfx/gui/SpriteSheet.h>
#include <cppfx/gui/BitmapFont.h>
#include <cppfx/WeakMap.h>

namespace cppfx {
	namespace gui {
		namespace elements{
			class CPPFX_API Document;
		}
		class CPPFX_API SpriteCollection : public Object {
		public:
			SpriteCollection(const ref_ptr<graphics::Context>& context, const string& basePath);
			bool contains(const string& key);
			bool containsFont(const string& key);
			ref_ptr<BitmapFont> loadBitmapFont(const string& href);
			ref_ptr<SpriteSheet> loadSpriteSheet(const string& href);
			void addFont(const string& key, const ref_ptr<BitmapFont>& font);
			void addSpriteSheet(const string& key, const ref_ptr<SpriteSheet>& spriteSheet);
			bool tryGetSprite(const string& key, SlicedSprite& sprite, ref_ptr<graphics::Texture2D>& texture);
			bool tryGetFont(const string& key, ref_ptr<BitmapFont>& font);
			const ref_ptr<graphics::Context>& getGraphicsContext();
		private:
			ref_ptr<graphics::Context> context;
			string basePath;
			std::unordered_map<string, ref_ptr<SpriteSheet> > spriteSheets;
			std::unordered_map<string, ref_ptr<BitmapFont > > fonts;
			WeakMap<string, graphics::Texture2D> cachedTextures;
			std::set<string> spriteFileMisses;

			friend class elements::Document;
		};
	}
}

#endif