#pragma once
#ifndef __CPPFX_GUI_SPRITESHEET_H
#define __CPPFX_GUI_SPRITESHEET_H
#include <cppfx/Object.h>
#include <cppfx/graphics/Context.h>

namespace cppfx
{
	namespace gui
	{
		struct CPPFX_API SlicedSprite
		{
			SlicedSprite();
			SlicedSprite(const SlicedSprite&);
			SlicedSprite& operator = (const SlicedSprite&);

			bool operator == (const SlicedSprite& s);
			bool operator != (const SlicedSprite& s);

			vector4i rect;
			vector4i edges;
		};
		class CPPFX_API SpriteSheet : public Object
		{
		public:
			SpriteSheet(const ref_ptr<graphics::Context>& context);
			
			void addSlicedSprite(const string& name, const SlicedSprite& spr);
			void addSlicedSprite(const string& name, const vector4i& rect, const vector4i& edges);
			void addSlicedSprite(const string& name, const string& sprite, const vector4i& edges);
		
			bool getSlicedSprite(const string& name, SlicedSprite& sprite);
			
			void setTexture(const ref_ptr<graphics::Texture2D>& texture);
			const ref_ptr<graphics::Texture2D>& getTexture();
			
			static ref_ptr<SpriteSheet> loadSpriteSheet(ref_ptr<graphics::Context> context, const string& fileName);
		private:
			SpriteSheet(const SpriteSheet&) = delete;
			SpriteSheet& operator = (const SpriteSheet&) = delete;
			string basePath;
			void readAtlasFromXml(const string& filename);
			ref_ptr<graphics::Context> context;
			ref_ptr<graphics::Texture2D> texture;
			std::unordered_map<string,SlicedSprite> slicedSprites;
		};
	}
}

#endif