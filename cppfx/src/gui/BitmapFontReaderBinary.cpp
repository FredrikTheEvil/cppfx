#include <cppfx/gui/BitmapFont.h>
#include <fstream>
#include <sstream>
#include <cppfx/Exceptions.h>

namespace cppfx
{
	namespace gui
	{
		struct bmfont_info_t
		{
			short fontSize;
			unsigned char flags;
			unsigned char charset;
			unsigned short stretchH;
			unsigned char aa;
			unsigned char paddingUp;
			unsigned char paddingRight;
			unsigned char paddingDown;
			unsigned char paddingLeft;
			unsigned char spacingHoriz;
			unsigned char spacingVert;
			unsigned char outline;
			string name;

			void read(std::istream& is, size_t size, unsigned char version = 3)
			{
				size_t blockSize = 13;

				is.read(reinterpret_cast<char*>(&fontSize), 2);
				is.read(reinterpret_cast<char*>(&flags), 1);
				is.read(reinterpret_cast<char*>(&charset), 1);
				is.read(reinterpret_cast<char*>(&stretchH), 2);
				is.read(reinterpret_cast<char*>(&aa), 1);
				is.read(reinterpret_cast<char*>(&paddingUp), 1);
				is.read(reinterpret_cast<char*>(&paddingRight), 1);
				is.read(reinterpret_cast<char*>(&paddingDown), 1);
				is.read(reinterpret_cast<char*>(&paddingLeft), 1);
				is.read(reinterpret_cast<char*>(&spacingHoriz), 1);
				is.read(reinterpret_cast<char*>(&spacingVert), 1);
				
				if (version >= 2) {
					is.read(reinterpret_cast<char*>(&outline), 1);
					blockSize++;
				}
				else
					throw RuntimeError("bmfont files must be version 3 or higher");

				std::vector<char> nameBuffer(size - blockSize);
				is.read(nameBuffer.data(), size - blockSize);
				name = string(nameBuffer.begin(), nameBuffer.end());
			}
		};
		struct bmfont_common_t
		{
			unsigned short lineHeight;
			unsigned short base;
			unsigned short scaleW;
			unsigned short scaleH;
			unsigned short pages;
			unsigned char flags;
			unsigned char alphaChnl;
			unsigned char redChnl;
			unsigned char greenChnl;
			unsigned char blueChnl;

			void read(std::istream& is, size_t size, unsigned char version = 3)
			{
				is.read(reinterpret_cast<char*>(&lineHeight), 2);
				is.read(reinterpret_cast<char*>(&base), 2);
				is.read(reinterpret_cast<char*>(&scaleW), 2);
				is.read(reinterpret_cast<char*>(&scaleH), 2);
				is.read(reinterpret_cast<char*>(&pages), 2);
				is.read(reinterpret_cast<char*>(&flags), 1);
				if (version >= 3)
				{
					is.read(reinterpret_cast<char*>(&alphaChnl), 1);
					is.read(reinterpret_cast<char*>(&redChnl), 1);
					is.read(reinterpret_cast<char*>(&greenChnl), 1);
					is.read(reinterpret_cast<char*>(&blueChnl), 1);
				}
				else
				{
					throw RuntimeError("bmfont files must be version 3 or higher");
				}
			}
		};
		struct bmfont_char_t
		{
			unsigned int id;
			unsigned short x;
			unsigned short y;
			unsigned short width;
			unsigned short height;
			short xoffset;
			short yoffset;
			short xadvance;
			unsigned char page;
			unsigned char chnl;
		};
		struct bmfont_kerning_pair_t
		{
			unsigned int first;
			unsigned int second;
			short amount;
		};

		void BitmapFont::readFromBinary(const string& filename, const string& buffer)
		{
			fileName = filename;
			std::stringstream ifs(buffer);
			bmfont_info_t info;
			bmfont_common_t common;

			char signature[4] = { 0, 0, 0, 0 };
			char version = 0;
			ifs.read(signature, 3);
			ifs.read(&version, 1);
			string sign = signature;
			if (sign != "BMF")
				throw RuntimeError("not a bmfont binary file");
			unsigned char blockId;
			unsigned int blockSize;
			while (!ifs.eof())
			{
				ifs.read(reinterpret_cast<char*>(&blockId), 1);
				ifs.read(reinterpret_cast<char*>(&blockSize), 4);

				if (blockId == 1)
				{
					// Info block
					info.read(ifs, blockSize, version);
				}
				else if (blockId == 2)
				{
					// Common block
					common.read(ifs, blockSize, version);
					base = common.base;
					lineHeight = common.lineHeight;
				}
				else if (blockId == 3)
				{
					// Pages block
					size_t pagesLen = blockSize / common.pages;
					std::vector<char> buffer(pagesLen);
					for (int i = 0; i < common.pages; i++)
					{
						ifs.read(buffer.data(), pagesLen);
						string pageName(buffer.begin(), buffer.end());
						addPage(pageName);
					}
				}
				else if (blockId == 4)
				{
					// Chars block
					size_t numChars = blockSize / 20;
					bmfont_char_t glyph;
					for (size_t i = 0; i < numChars; i++)
					{
						ifs.read(reinterpret_cast<char*>(&glyph.id), 4);
						ifs.read(reinterpret_cast<char*>(&glyph.x), 2);
						ifs.read(reinterpret_cast<char*>(&glyph.y), 2);
						ifs.read(reinterpret_cast<char*>(&glyph.width), 2);
						ifs.read(reinterpret_cast<char*>(&glyph.height), 2);
						ifs.read(reinterpret_cast<char*>(&glyph.xoffset), 2);
						ifs.read(reinterpret_cast<char*>(&glyph.yoffset), 2);
						ifs.read(reinterpret_cast<char*>(&glyph.xadvance), 2);
						ifs.read(reinterpret_cast<char*>(&glyph.page), 1);
						ifs.read(reinterpret_cast<char*>(&glyph.chnl), 1);

						BitmapFontGlyph fontGlyph;
						fontGlyph.id = glyph.id;
						fontGlyph.x = glyph.x;
						fontGlyph.y = glyph.y;
						fontGlyph.width = glyph.width;
						fontGlyph.height = glyph.height;
						fontGlyph.xoffset = glyph.xoffset;
						fontGlyph.yoffset = glyph.yoffset;
						fontGlyph.xadvance = glyph.xadvance;
						fontGlyph.page = glyph.page;
						//fontGlyph.chnl = glyph.chnl;
						if (glyph.id == 0xffffffff)
							setGlyphUnknown(fontGlyph);
						else
							addGlyph(fontGlyph);
					}
				}
				else if (blockId == 5)
				{
					size_t numKerningPairs = blockSize / 10;
					// Kerning pairs block
					bmfont_kerning_pair_t kp;
					for (size_t i = 0; i < numKerningPairs; i++)
					{
						ifs.read(reinterpret_cast<char*>(&kp.first), 4);
						ifs.read(reinterpret_cast<char*>(&kp.second), 4);
						ifs.read(reinterpret_cast<char*>(&kp.amount), 2);
						BitmapFontKerningPair bmkp;
						bmkp.first = kp.first;
						bmkp.second = kp.second;
						bmkp.amount = kp.amount;
						addKerningPair(bmkp);
					}
				}
			}
		}
	}
}