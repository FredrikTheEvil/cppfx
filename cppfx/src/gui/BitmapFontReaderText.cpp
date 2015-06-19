#include <cppfx/gui/BitmapFont.h>
#include <fstream>
#include <regex>
#include <sstream>

namespace cppfx
{
	namespace gui
	{
		void BitmapFont::readFromText(const string& filename, const string& buffer)
		{
			int state = 0;
			int curPage = 0;
			int curChar = 0;
			int pageCount = 0;
			int numChars = 0;
			int curKerning = 0;
			int numKernings = 0;

			std::stringstream ifs(buffer);
			string line = "";
			string fileName;
			while (!ifs.eof())
			{
				std::getline(ifs, line);
				if (state == 0) {
					if (line.size() > 0)
					{
						string::size_type pos = line.find_first_of('"');
						string::size_type end = string::npos;
						if (pos != string::npos)
						{
							end = line.find_first_of('"', pos+1);
						}
						name = line.substr(pos+1, end-1-pos);
						string format = "info face=\"" + name + "\" size=%d";
#if defined(CPPFX_HAVE_SSCANF_S)
						sscanf_s(line.data(), format.data(), &size);
#else
						sscanf(line.data(), format.data(), &size);
#endif
						state++;
					}
				}
				else if (state == 1)
				{
					if (line.size() > 0)
					{
						int scaleW = 0, scaleH = 0;
#if defined(CPPFX_HAVE_SSCANF_S)
						sscanf_s(line.data(), "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d", &lineHeight, &base, &scaleW, &scaleH, &pageCount);
#else
						sscanf(line.data(), "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d", &lineHeight, &base, &scaleW, &scaleH, &pageCount);
#endif
						pages.reserve(pageCount);
						state++;
					}
				}
				else if (state == 2) {
					if (line.size() > 0)
					{
						string::size_type pos = line.find_first_of('"');
						string::size_type end = string::npos;
						if (pos != string::npos)
						{
							end = line.find_first_of('"', pos+1);
						}
						fileName = line.substr(pos + 1, end - 1 - pos);
						addPage(fileName);
						curPage++;
					}
					if (curPage >= pageCount)
						state++;
				}
				else if (state == 3)
				{
					if (line.size() > 0)
					{
#if defined(CPPFX_HAVE_SSCANF_S)
						sscanf_s(line.data(), "chars count=%d", &numChars);
#else
						sscanf(line.data(), "chars count=%d", &numChars);
#endif
						glyphs.reserve(numChars);
						state++;
					}
				}
				else if (state == 4) {
					if (line.size() > 0)
					{
						int id;
						BitmapFontGlyph glyph;
#if defined(CPPFX_HAVE_SSCANF_S)
						sscanf_s(line.data(), "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d"
							, &id, &glyph.x, &glyph.y, &glyph.width, &glyph.height, &glyph.xoffset, &glyph.yoffset, &glyph.xadvance, &glyph.page);
#else
						sscanf(line.data(), "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d"
							, &id, &glyph.x, &glyph.y, &glyph.width, &glyph.height, &glyph.xoffset, &glyph.yoffset, &glyph.xadvance, &glyph.page);
#endif
						if (id == -1)
						{
							glyph.id = 0xffffffff;
							setGlyphUnknown(glyph);
						}
						else
						{
							glyph.id = id;
							addGlyph(glyph);
						}
						curChar++;
					}
					if (curChar >= numChars)
						state++;
				}
				else if (state == 5)
				{
					if (line.size() > 0)
					{
#if defined(CPPFX_HAVE_SSCANF_S)
						sscanf_s(line.data(), "kernings count=%d", &numKernings);
#else
						sscanf(line.data(), "kernings count=%d", &numKernings);
#endif
						kerningPairs.reserve(numKernings);
						state++;
					}
				}
				else if (state == 6)
				{
					if (line.size() > 0)
					{
						int first = 0, second = 0, amount = 0;
#if defined(CPPFX_HAVE_SSCANF_S)
						sscanf_s(line.data(), "kerning first=%d second=%d amount=%d", &first, &second, &amount);
#else
						sscanf(line.data(), "kerning first=%d second=%d amount=%d", &first, &second, &amount);
#endif
						BitmapFontKerningPair kp;
						kp.first = first;
						kp.second = second;
						kp.amount = amount;
						addKerningPair(kp);
						curKerning++;
					}
					if (curKerning >= numKernings)
						state++;
				}
			}
		}
	}
}