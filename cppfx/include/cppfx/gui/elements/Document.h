#pragma once
#ifndef __CPPFX_GUI_DOCUMENT_H
#define __CPPFX_GUI_DOCUMENT_H
#include <cppfx/gui/elements/Element.h>
#include <cppfx/gui/SpriteCollection.h>
#include <cppfx/gui/elements/IElementFactory.h>
#include <cppfx/graphics/Context.h>
#include <cppfx/weak_ptr.h>
#include <cppfx/WeakMap.h>

#include <cppfx/gui/elements/HeadElement.h>
#include <cppfx/gui/elements/BodyElement.h>

namespace cppfx
{
	namespace gui
	{
		namespace elements {
			class CPPFX_API Gui;
			class CPPFX_API Element;
			class CPPFX_API LinkElement;
			class CPPFX_API Document : public Element
			{
			public:
				static ref_ptr<Document> loadDocument(const ref_ptr<graphics::Context>& context, const string& fileName);
				static ref_ptr<Document> loadDocumentFromXml(const ref_ptr<graphics::Context>& graphics, const string& fileName, const xml::node& document);
				
				explicit Document(const ref_ptr<graphics::Context>& context, const string& basePath);

				void setScreenSize(const vector2i& screenSize);
				void render();

				const ref_ptr<HeadElement>& getHead();
				const ref_ptr<BodyElement>& getBody();

				const string& getBasePath() const;

				void registerFont(const string& tag, const ref_ptr<BitmapFont>& font);
				static void registerElement(const string& tag, const ref_ptr<elements::IElementFactory>& factory);

				bool eventHandler(const platform::Event& event);
			protected:
				ref_ptr<Element> createElementI(const string& elementType, const ref_ptr<Element>& parent);
				virtual const ref_ptr<WidgetManager>& widgetManager() override;
				virtual Document * getDocument() override;

				virtual ref_ptr<BitmapFont> resolveFont(const string& name) override;
				virtual bool resolveSprite(const string& name, SlicedSprite& sprite, ref_ptr<graphics::Texture2D>& texture) override;
			private:
				ref_ptr<BitmapFont> loadFont(const string& href);
				ref_ptr<SpriteSheet> loadSpriteSheet(const string& href);
				void setFocusElement(const ref_ptr<Element>& element);
				void dispatchEvent(const GuiEvent& event) override;
				virtual ~Document();
				string basePath;
				ref_ptr<graphics::Context> context;
				ref_ptr<GuiShader> shader;
				ref_ptr<SpriteBatch> spriteBatch;
				ref_ptr<WidgetManager> manager;
				ref_ptr<HeadElement> head;
				ref_ptr<BodyElement> body;
				std::map<string, ref_ptr<BitmapFont> > fonts;
				ref_ptr<SpriteCollection> spriteCollection;
				static std::map<string, ref_ptr<elements::IElementFactory> > elementFactories;
				weak_ptr<Element> focusElement;
				WeakMap<string, Element> idCache;
				std::unordered_map<string, std::vector<Element*> > classCache;
				platform::EventHandler boundEventHandler;
				vector2i screenSize;
				bool dirtyLayout;
				bool needRedraw = false; // Require redrawing gui
				Document(const Document&) = delete;
				Document& operator = (const Document&) = delete;

				friend class Element;
				friend class LinkElement;
			};
		}
	}
}

#endif