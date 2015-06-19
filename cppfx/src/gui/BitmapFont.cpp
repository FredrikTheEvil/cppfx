#include <cppfx/gui/BitmapFont.h>
#include <cppfx/gui/SpriteCollection.h>
#include <fstream>
#include <cstdarg>
#include <sstream>
#include <cppfx/Exceptions.h>

namespace cppfx
{
	namespace gui
	{
		BitmapFontGlyph::BitmapFontGlyph() :
			id(0),
			x(0),
			y(0),
			width(0),
			height(0),
			xoffset(0),
			yoffset(0),
			xadvance(0),
			page(0)
		{
		}

		BitmapFontGlyph::BitmapFontGlyph(const BitmapFontGlyph& glyph) :
			id(glyph.id),
			x(glyph.x),
			y(glyph.y),
			width(glyph.width),
			height(glyph.height),
			xoffset(glyph.xoffset),
			yoffset(glyph.yoffset),
			xadvance(glyph.xadvance),
			page(glyph.page)
		{
		}

		BitmapFontGlyph& BitmapFontGlyph::operator=(const BitmapFontGlyph& glyph)
		{
			id = glyph.id;
			x = glyph.x;
			y = glyph.y;
			width = glyph.width;
			height = glyph.height;
			xoffset = glyph.xoffset;
			yoffset = glyph.yoffset;
			xadvance = glyph.xadvance;
			page = glyph.page;

			return *this;
		}
		BitmapFontPage::BitmapFontPage() :
			texture(nullptr),
			sprite()
		{
		}
		BitmapFontPage::BitmapFontPage(const BitmapFontPage& page) :
			texture(page.texture),
			sprite(page.sprite)
		{
		}
		BitmapFontPage& BitmapFontPage::operator=(const BitmapFontPage& page)
		{
			texture = page.texture;
			sprite = page.sprite;
			return *this;
		}
		BitmapFontKerningPair::BitmapFontKerningPair() :
			first(0),
			second(0),
			amount(0)
		{
		}
		BitmapFontKerningPair::BitmapFontKerningPair(const BitmapFontKerningPair& kp) :
			first(kp.first),
			second(kp.second),
			amount(kp.amount)
		{
		}

		BitmapFontKerningPair& BitmapFontKerningPair::operator=(const BitmapFontKerningPair& kp)
		{
			first = kp.first;
			second = kp.second;
			amount = kp.amount;
			return *this;
		}

		BitmapFont::BitmapFont(ref_ptr<graphics::Context> context) :
			context(context),
			spriteCollection(nullptr)
		{
		}

		BitmapFont::BitmapFont(SpriteCollection* spriteSheet) :
			context(spriteSheet->getGraphicsContext()),
			spriteCollection(spriteSheet)
		{
			spriteCollection->ref();
		}
		BitmapFont::~BitmapFont() {
			if (spriteCollection != nullptr)
				spriteCollection->unref();
		}

		ref_ptr<BitmapFont> BitmapFont::loadBitmapFont(SpriteCollection* spriteSheet, const string& fileName)
		{
			ref_ptr<BitmapFont> font = new BitmapFont(spriteSheet);
			font->read(fileName);
			return font;
		}

		ref_ptr<BitmapFont> BitmapFont::loadBitmapFont(const ref_ptr<graphics::Context>& context, const string& fileName)
		{
			ref_ptr<BitmapFont> font = nullptr;
			font = new BitmapFont(context);
			font->read(fileName);
			return font;
		}

		void BitmapFont::setName(const string& name) {
			BitmapFont::name = name;
		}
		const string& BitmapFont::getName() const {
			return name;
		}

		void BitmapFont::render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const wstring& text)
		{
			for (auto ch = text.begin(); ch != text.end(); ++ch)  {
				char32_t id(*ch);
				const BitmapFontGlyph& glyph = getGlyph(id);
				if (glyph.page < 0 || size_t(glyph.page) >= pages.size())
					throw RuntimeError("page index out of range");
				const BitmapFontPage& page = pages[glyph.page];
				emit(spriteBatch, rect, pos, color, glyph, page);
			}
		}

		void BitmapFont::render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const string_utf32& text)
		{
			for (auto ch = text.begin(); ch != text.end(); ++ch)  {
				char32_t id(*ch);
				const BitmapFontGlyph& glyph = getGlyph(id);
				if (glyph.page < 0 || size_t(glyph.page) >= pages.size())
					throw RuntimeError("page index out of range");
				const BitmapFontPage& page = pages[glyph.page];
				emit(spriteBatch, rect, pos, color, glyph, page);
			}
		}

		void BitmapFont::render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const string_utf16& text)
		{
			for (auto ch = text.begin(); ch != text.end(); ++ch)  {
				char32_t id(*ch);
				const BitmapFontGlyph& glyph = getGlyph(id);
				if (glyph.page < 0 || size_t(glyph.page) >= pages.size())
					throw RuntimeError("page index out of range");
				const BitmapFontPage& page = pages[glyph.page];
				emit(spriteBatch, rect, pos, color, glyph, page);
			}
		}

		typedef std::basic_stringstream< char32_t, std::char_traits<char32_t>, std::allocator<char32_t> > u32stringstream;

		void BitmapFont::render(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const string& text)
		{
			for (auto ch = text.begin(); ch != text.end(); ++ch)  {
				char32_t id(*ch);
				const BitmapFontGlyph& glyph = getGlyph(id);
				if (glyph.page < 0 || size_t(glyph.page) >= pages.size())
					throw RuntimeError("page index out of range");
				const BitmapFontPage& page = pages[glyph.page];
				emit(spriteBatch, rect, pos, color, glyph, page);
			}
		}

		size_t BitmapFont::getNumPages() const
		{
			return pages.size();
		}
		size_t BitmapFont::getNumGlyphs() const
		{
			return glyphs.size();
		}

		void BitmapFont::setBase(int base)
		{
			BitmapFont::base = base;
		}

		void BitmapFont::setLineHeight(int lineHeight)
		{
			BitmapFont::lineHeight = lineHeight;
		}

		int BitmapFont::getBase() const
		{
			return base;
		}

		int BitmapFont::getLineHeight() const
		{
			return lineHeight;
		}

		void BitmapFont::addPage(const string& pageName)
		{
			if (spriteCollection != nullptr)
			{
				SlicedSprite sprite;
				ref_ptr<graphics::Texture2D> tex;
				if (spriteCollection->tryGetSprite(pageName, sprite, tex)) {
					BitmapFontPage page;
					page.texture = tex;
					page.sprite = sprite;
					pages.push_back(page);
					return;
				}
			}
			string fileName = basePath;
			if (fileName.size() > 0)
				fileName = fileName + pageName;
			else
				fileName = pageName;

#ifdef WIN32
			for (auto i = fileName.begin(); i != fileName.end(); ++i)
			{
				if (*i == '/')
					*i = '\\';
			}
#endif
			ref_ptr<graphics::Texture2D> texture = context->loadTexture2D(fileName);
			BitmapFontPage page;
			page.texture = texture;
			page.sprite.rect = vector4i(0, 0, texture->getWidth(), texture->getHeight());
			pages.push_back(page);
		}
		void BitmapFont::setGlyphUnknown(const BitmapFontGlyph& glyph)
		{
			unknownGlyph = glyph;
		}
		void BitmapFont::addGlyph(const BitmapFontGlyph& glyph)
		{
			glyphs.push_back(glyph);
		}

		void BitmapFont::addKerningPair(const BitmapFontKerningPair& kerningPair)
		{
			kerningPairs.push_back(kerningPair);
		}

		const BitmapFontGlyph& BitmapFont::getGlyph(char32_t id) const
		{
			for (auto g = glyphs.begin(); g != glyphs.end(); ++g)
				if (g->id == id)
					return *g;
			return unknownGlyph;
		}

		void BitmapFont::emit(SpriteBatchSprite& s, const vector4f& rect, vector2f& pos, const colorf& color, const BitmapFontGlyph& glyph, const BitmapFontPage& page)
		{
			if (pos.x + glyph.width > rect.x + rect.z)
			{
				if (pos.x == rect.x)
				{
					// The rect is just too damn small, dont bother
					return;
				}
				pos.x = rect.x;
				pos.y += lineHeight;
			}
			if (pos.y + glyph.height + glyph.yoffset > rect.y + rect.w)
				return;

			float pageWidth = float(page.texture->getWidth());
			float pageHeight = float(page.texture->getHeight());

			int left = page.sprite.rect.x + glyph.x;
			int right = page.sprite.rect.x + glyph.x + glyph.width;
			int top = page.sprite.rect.y + glyph.y;
			int bottom = page.sprite.rect.y + glyph.y + glyph.height;

			float posLeft = glyph.xoffset + pos.x;
			float posTop = glyph.yoffset + pos.y;
			float posRight = glyph.xoffset + pos.x + glyph.width;
			float posBottom = glyph.yoffset + pos.y + glyph.height;

			s.topLeft.pos = vector2f(posLeft, posTop);
			s.topLeft.uv = vector2f(float(left) / float(pageWidth), float(top) / float(pageHeight));
			s.topLeft.color = color;
			s.topRight.pos = vector2f(posRight, posTop);
			s.topRight.uv = vector2f(float(right) / float(pageWidth), float(top) / float(pageHeight));
			s.topRight.color = color;
			s.bottomLeft.pos = vector2f(posLeft, posBottom);
			s.bottomLeft.uv = vector2f(float(left) / float(pageWidth), float(bottom) / float(pageHeight));
			s.bottomLeft.color = color;
			s.bottomRight.pos = vector2f(posRight, posBottom);
			s.bottomRight.uv = vector2f(float(right) / float(pageWidth), float(bottom) / float(pageHeight));
			s.bottomRight.color = color;
			pos.x += glyph.xadvance;
		}

		void BitmapFont::emit(const ref_ptr<SpriteBatch>& spriteBatch, const vector4f& rect, vector2f& pos, const colorf& color, const BitmapFontGlyph& glyph, const BitmapFontPage& page)
		{
			if (pos.x + glyph.width > rect.x + rect.z)
			{
				if (pos.x == rect.x)
				{
					// The rect is just too damn small, dont bother
					return;
				}
				pos.x = rect.x;
				pos.y += lineHeight;
			}
			if (pos.y + glyph.height + glyph.yoffset > rect.y + rect.w)
				return;

			float pageWidth = float(page.texture->getWidth());
			float pageHeight = float(page.texture->getHeight());

			int left = page.sprite.rect.x + glyph.x;
			int right = page.sprite.rect.x + glyph.x + glyph.width;
			int top = page.sprite.rect.y + glyph.y;
			int bottom = page.sprite.rect.y + glyph.y + glyph.height;

			float posLeft = glyph.xoffset + pos.x;
			float posTop = glyph.yoffset + pos.y;
			float posRight = glyph.xoffset + pos.x + glyph.width;
			float posBottom = glyph.yoffset + pos.y + glyph.height;

			SpriteBatchSprite s;
			s.topLeft.pos = vector2f(posLeft, posTop);
			s.topLeft.uv = vector2f(float(left) / float(pageWidth), float(top) / float(pageHeight));
			s.topLeft.color = color;
			s.topRight.pos = vector2f(posRight, posTop);
			s.topRight.uv = vector2f(float(right) / float(pageWidth), float(top) / float(pageHeight));
			s.topRight.color = color;
			s.bottomLeft.pos = vector2f(posLeft, posBottom);
			s.bottomLeft.uv = vector2f(float(left) / float(pageWidth), float(bottom) / float(pageHeight));
			s.bottomLeft.color = color;
			s.bottomRight.pos = vector2f(posRight, posBottom);
			s.bottomRight.uv = vector2f(float(right) / float(pageWidth), float(bottom) / float(pageHeight));
			s.bottomRight.color = color;
			pos.x += glyph.xadvance;

			spriteBatch->push(s, page.texture);
		}

		bool isBinaryBmfont(const string& buffer)
		{
			if (buffer[0] == 'B' && buffer[1] == 'M' && buffer[2] == 'F' && buffer[3] >= 3)
				return true;
			return false;
		}
		bool isXmlBmfont(const string& buffer)
		{
			if (buffer.find("<?xml") == 0)
				return true;
			return false;
		}
		bool isTextBmfont(const string& buffer)
		{
			if (buffer.find("info") == 0)
				return true;
			return false;
		}

		void BitmapFont::read(const string& filename)
		{
			string filePath = filename;
			for (auto i = filePath.begin(); i != filePath.end(); ++i)
			{
				if (*i == '\\')
					*i = '/';
			}
			fileName = filePath;

			auto lastPathSeparator = filePath.find_last_of('/');
			if (lastPathSeparator != string::npos)
				basePath = filePath.substr(0, lastPathSeparator) + "/";
			else
				basePath = "";
#ifdef WIN32
			for (auto i = filePath.begin(); i != filePath.end(); ++i)
			{
				if (*i == '/')
					*i = '\\';
			}
#endif
			std::ifstream ifs(filename, std::ios_base::in | std::ios_base::binary);
			string buffer = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
			if (isBinaryBmfont(buffer))
				readFromBinary(filename, buffer);
			else if (isXmlBmfont(buffer))
				readFromXml(filename, buffer);
			else if (isTextBmfont(buffer))
				readFromText(filename, buffer);
			else
				throw RuntimeError("unknown bitmap font format");
		}
	}
}