#include <cppfx/gui/SpriteSheet.h>
#include <cppfx/Xml.h>
#include <fstream>
#include <cppfx/gui/BitmapFont.h>

namespace cppfx
{
	namespace gui
	{
		SlicedSprite::SlicedSprite() : 
			rect(),
			edges()
		{
		}

		SlicedSprite::SlicedSprite(const SlicedSprite& s) :
			rect(s.rect),
			edges(s.edges)
		{
		}

		SlicedSprite& SlicedSprite::operator=(const SlicedSprite& s)
		{
			rect = s.rect;
			edges = s.edges;
			return *this;
		}

		bool SlicedSprite::operator==(const SlicedSprite& s)
		{
			return s.rect == rect &&
				s.edges == edges;
		}

		bool SlicedSprite::operator!=(const SlicedSprite& s)
		{
			return s.rect != rect ||
				s.edges != edges;
		}

		SpriteSheet::SpriteSheet(const ref_ptr<graphics::Context>& context) : 
			context(context)
		{
		}

		void SpriteSheet::addSlicedSprite(const string& name, const SlicedSprite& spr)
		{
			slicedSprites.insert(std::pair<string, SlicedSprite>(name, spr));
		}

		void SpriteSheet::addSlicedSprite(const string& name, const vector4i& rect, const vector4i& edges)
		{
			SlicedSprite spr;
			spr.rect = rect;
			spr.edges = edges;
			slicedSprites.insert(std::pair<string, SlicedSprite>(name, spr));
		}

		void SpriteSheet::addSlicedSprite(const string& name, const string& sprite, const vector4i& edges)
		{
			SlicedSprite sheetSprite;
			if (getSlicedSprite(sprite, sheetSprite))
			{
				SlicedSprite spr;
				spr.rect = sheetSprite.rect;
				spr.edges = edges;
				slicedSprites.insert(std::pair<string, SlicedSprite>(name, spr));
			}
		}

		bool SpriteSheet::getSlicedSprite(const string& name, SlicedSprite& sprite)
		{
			auto p = slicedSprites.find(name);
			if (p != slicedSprites.end())
			{
				sprite = p->second;
				return true;
			}
			return false;
		}

		void SpriteSheet::setTexture(const ref_ptr<graphics::Texture2D>& texture)
		{
			SpriteSheet::texture = texture;
		}

		const ref_ptr<graphics::Texture2D>& SpriteSheet::getTexture()
		{
			return texture;
		}

		//void read_texture_atlas(const ref_ptr<graphics::Context> context, const ref_ptr<SpriteSheet>& sheet, )

		ref_ptr<SpriteSheet> SpriteSheet::loadSpriteSheet(ref_ptr<graphics::Context> context, const string& fileName)
		{
			std::ifstream ifs(fileName);
			std::string xmlData = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

			xml::document doc;
			doc.parse<0>(const_cast<char*>(get_string_buffer(xmlData)));
			auto sheetNode = doc.first_node("SpriteSheet", 11);
			string basePath = "";
			if (sheetNode != nullptr)
			{
				auto atlas = xml::getAttribValue(sheetNode->first_attribute("textureAtlas"));
				ref_ptr<SpriteSheet> sheet = new SpriteSheet(context);
				sheet->basePath = basePath;
				if (atlas.size() > 0) {
					sheet->readAtlasFromXml(basePath + atlas);
				} 
				else
				{
					auto textureFileName = xml::getAttribValue(sheetNode->first_attribute("texture", 7));
					if (textureFileName.size() == 0)
						throw std::runtime_error("SpriteSheet requires either a texture or a texture atlas");
					ref_ptr<graphics::Texture2D> texture = context->loadTexture2D(basePath + textureFileName);
					if (texture.valid())
						sheet->setTexture(texture);	
				}
				auto spriteNode = sheetNode->first_node("Sprite", 6);
				while (spriteNode != nullptr)
				{
					auto name = xml::getAttribValue(spriteNode->first_attribute("name"));
					if (name.size() > 0)
					{
					}
					spriteNode = spriteNode->next_sibling("Sprite", 6);
				}
				auto slicedSpriteNode = sheetNode->first_node("SlicedSprite", 12);
				while (slicedSpriteNode != nullptr)
				{
					auto sprite = xml::getAttribValue(slicedSpriteNode->first_attribute("sprite"));
					auto name = xml::getAttribValue(slicedSpriteNode->first_attribute("name"));

					if (name.size()) {
						auto left = xml::getAttribInt(slicedSpriteNode, "left");
						auto right = xml::getAttribInt(slicedSpriteNode, "right");
						auto top = xml::getAttribInt(slicedSpriteNode, "top");
						auto bottom = xml::getAttribInt(slicedSpriteNode, "bottom");

						vector4i edges(left, top, right, bottom);

						if (sprite.size() > 0) {
							sheet->addSlicedSprite(name, sprite, vector4i(left, top, right, bottom));
						}
						else
						{
							auto ofs_x = xml::getAttribInt(slicedSpriteNode, "x");
							auto ofs_y = xml::getAttribInt(slicedSpriteNode, "y");
							auto width = xml::getAttribInt(slicedSpriteNode, "width");
							auto height = xml::getAttribInt(slicedSpriteNode, "height");
							vector4i rect(ofs_x, ofs_y, width, height);

							sheet->addSlicedSprite(name, rect, edges);
						}
					}
					slicedSpriteNode = slicedSpriteNode->next_sibling("SlicedSprite", 12);
				}
				return sheet;
			}
			throw std::runtime_error("invalid spritesheet");
		}

		void SpriteSheet::readAtlasFromXml(const string& filename)
		{
			using namespace rapidxml;
			string atlasPath = filename;
			if (basePath.size() > 0)
				atlasPath = basePath + atlasPath;
#ifdef WIN32
			for (auto i = atlasPath.begin(); i != atlasPath.end(); ++i)
			{
				if (*i == '/')
					*i = '\\';
			}
#endif
			std::ifstream ifs(atlasPath);
			std::string buffer = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

			xml::document doc;
			doc.parse<0>(const_cast<char*>(get_string_buffer(buffer)));
			xml::node* rootNode = doc.first_node("TextureAtlas", 12);
			if (rootNode != nullptr) {
				string imagePath = xml::getAttribValue(rootNode->first_attribute("imagePath", 9));
				if (basePath.size() > 0)
					imagePath = basePath + imagePath;

#ifdef WIN32
				for (auto i = imagePath.begin(); i != imagePath.end(); ++i)
				{
					if (*i == '/')
						*i = '\\';
				}
#endif
				ref_ptr<graphics::Texture2D> tex = context->loadTexture2D(imagePath);
				setTexture(tex);

				xml::node* sprite = rootNode->first_node("sprite", 6);
				while (sprite != nullptr)
				{
					string name = xml::getAttribValue(sprite->first_attribute("n", 1));
					SlicedSprite s;
					s.rect = vector4i(
						xml::getAttribInt(sprite, "x"),
						xml::getAttribInt(sprite, "y"),
						xml::getAttribInt(sprite, "w"),
						xml::getAttribInt(sprite, "h")
						);
					addSlicedSprite(name, s);

					sprite = sprite->next_sibling("sprite", 6);
				}
			}
		}
	}
}