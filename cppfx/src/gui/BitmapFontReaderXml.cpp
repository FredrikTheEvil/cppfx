#include <cppfx/gui/BitmapFont.h>
#include <cppfx/rapidxml.h>
#include <fstream>
#include <cppfx/Xml.h>

namespace cppfx
{
	namespace gui
	{

		void BitmapFont::readFromXml(const string& filename, const string& buffer)
		{
			using namespace rapidxml;
			fileName = filename;
			rapidxml::xml_document<> doc;
			doc.parse<0>(const_cast<char*>(get_string_buffer(buffer)));
			xml_node<>* fontNode = doc.first_node("font");
			if (fontNode == nullptr)
				throw std::runtime_error("invalid bmfont xml file");
			xml::node* infoNode = fontNode->first_node("info");
			xml::node* commonNode = fontNode->first_node("common");
			xml::node* pagesNode = fontNode->first_node("pages");
			xml::node* charsNode = fontNode->first_node("chars");
			xml::node* kerningsNode = fontNode->first_node("kernings");

			if (infoNode == nullptr || commonNode == nullptr || pagesNode == nullptr ||
				charsNode == nullptr)
				throw std::runtime_error("invalid bmfont xml file");
			xml_attribute<>* nameAttrib = infoNode->first_attribute("face", 4);
			name = xml::getAttribValue(nameAttrib);
			size = xml::getAttribInt(infoNode, "size");
			lineHeight = xml::getAttribInt(commonNode, "lineHeight");
			base = xml::getAttribInt(commonNode, "base");
			auto pageCount = xml::getAttribInt(commonNode, "pages");
			pages.reserve(pageCount);
			auto charCount = xml::getAttribInt(charsNode, "count");
			glyphs.reserve(charCount);

			auto ptr = pagesNode->first_node("page", 4);
			while (ptr != nullptr)
			{
				string file = xml::getAttribValue(ptr->first_attribute("file", 4));
				addPage(file);
				ptr = ptr->next_sibling("page");
			}
			if (kerningsNode != nullptr)
			{
				int numKernings = xml::getAttribInt(kerningsNode, "count");
				kerningPairs.reserve(numKernings);
				ptr = kerningsNode->first_node("kerning", 7);
				while (ptr != nullptr)
				{
					int first = xml::getAttribInt(ptr, "first");
					int second = xml::getAttribInt(ptr, "second");
					int amount = xml::getAttribInt(ptr, "amount");

					BitmapFontKerningPair kp;
					kp.first = first;
					kp.second = second;
					kp.amount = amount;
					addKerningPair(kp);

					ptr = ptr->next_sibling("kerning");
				}
			}
			ptr = charsNode->first_node("char", 4);
			while (ptr != nullptr)
			{
				int id = xml::getAttribInt(ptr, "id");				
				BitmapFontGlyph glyph;
				glyph.x = xml::getAttribInt(ptr, "x");
				glyph.y = xml::getAttribInt(ptr, "y");
				glyph.width = xml::getAttribInt(ptr, "width");
				glyph.height = xml::getAttribInt(ptr, "height");
				glyph.xoffset = xml::getAttribInt(ptr, "xoffset");
				glyph.yoffset = xml::getAttribInt(ptr, "yoffset");
				glyph.xadvance = xml::getAttribInt(ptr, "xadvance");
				glyph.page = xml::getAttribInt(ptr, "page");

				if (id == -1) 
				{
					glyph.id = 0xffffffff;
					setGlyphUnknown(glyph);
				}
				else {
					glyph.id = id;
					addGlyph(glyph);
				}
				ptr = ptr->next_sibling("char");
			}
		}
	}
}