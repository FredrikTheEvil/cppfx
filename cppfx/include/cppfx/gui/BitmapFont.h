#pragma once
#ifndef __CPPFX_GUI_BITMAPFONT_H
#define __CPPFX_GUI_BITMAPFONT_H
#include <cppfx/Object.h>
#include <vector>
#include <cppfx/graphics/Texture2D.h>
#include <cppfx/vector.h>
#include <cppfx/gui/SpriteSheet.h>
#include <cppfx/gui/SpriteBatch.h>

namespace cppfx
{
	namespace gui
	{
		class CPPFX_API SpriteCollection;
		struct CPPFX_API BitmapFontGlyph
		{
			BitmapFontGlyph();
			BitmapFontGlyph(const BitmapFontGlyph& glyph);
			BitmapFontGlyph& operator = (const BitmapFontGlyph& glyph);

			char32_t id;
			int x;
			int y;
			int width;
			int height;
			int xoffset;
			int yoffset;
			int xadvance;
			int page;
		};
		struct CPPFX_API BitmapFontPage
		{
			BitmapFontPage();
			BitmapFontPage(const BitmapFontPage& page);
			BitmapFontPage& operator = (const BitmapFontPage& page);

			ref_ptr<graphics::Texture2D> texture;
			SlicedSprite sprite;
		};
		struct CPPFX_API BitmapFontKerningPair
		{
			BitmapFontKerningPair();
			BitmapFontKerningPair(const BitmapFontKerningPair& kp);
			BitmapFontKerningPair& operator=(const BitmapFontKerningPair& kp);

			unsigned int first;
			unsigned int second;
			short amount;
		};
		class CPPFX_API LabelWidget;
		class CPPFX_API BitmapFont : public Object
		{
		public:
			BitmapFont(ref_ptr<graphics::Context> context);
			BitmapFont(SpriteCollection* spriteCollection);
			static ref_ptr<BitmapFont> loadBitmapFont(SpriteCollection* spriteCollection, const string& fileName);
			//static ref_ptr<BitmapFont> loadBitmapFont(const ref_ptr<graphics::Context>& context, const ref_ptr<SpriteSheet>& spriteSheet, const string& fileName);
			static ref_ptr<BitmapFont> loadBitmapFont(const ref_ptr<graphics::Context>& context, const string& fileName);

			void render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const wstring& text);
			void render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const string_utf32& text);
			void render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const string_utf16& text);
			void render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const string& text);

			size_t getNumPages() const;
			size_t getNumGlyphs() const;

			void setName(const string& name);
			const string& getName() const;

			void setBase(int base);
			void setLineHeight(int lineHeight);

			int getBase() const;
			int getLineHeight() const;
			
			void addPage(const string& pageName);
			void setGlyphUnknown(const BitmapFontGlyph& glyph);
			void addGlyph(const BitmapFontGlyph& glyph);
			void addKerningPair(const BitmapFontKerningPair& kerningPair);
			const BitmapFontGlyph& getGlyph(char32_t id) const;
			virtual ~BitmapFont();
		private:			
			void emit(SpriteBatchSprite& batchSprite, const vector4f& rect, vector2f& pos, const colorf& color, const BitmapFontGlyph& glyph, const BitmapFontPage& page);
			void emit(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const BitmapFontGlyph& glyph, const BitmapFontPage& page);

			BitmapFont(const BitmapFont&) = delete;
			BitmapFont& operator=(const BitmapFont&) = delete;

			void read(const string& filename);
			void readFromText(const string& filename, const std::string& buffer);
			void readFromBinary(const string& filename, const std::string& buffer);
			void readFromXml(const string& filename, const std::string& buffer);
			ref_ptr<graphics::Context> context;
			SpriteCollection* spriteCollection;

			string fileName;
			string basePath;
			string name;
			int size;
			int base;
			int lineHeight;
			BitmapFontGlyph unknownGlyph;
			std::vector<BitmapFontGlyph> glyphs;
			std::vector<BitmapFontPage> pages;
			std::vector<BitmapFontKerningPair> kerningPairs;

			friend class LabelWidget;
			friend class TextRenderer;
		};
	}
}

#endif