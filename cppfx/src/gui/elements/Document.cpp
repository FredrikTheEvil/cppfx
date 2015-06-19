#include <cppfx/gui/elements/Document.h>
#include <cppfx/gui/GuiEvent.h>
#include <fstream>

namespace cppfx
{
	WeakMap<string, Referenced> testMap;
	namespace gui
	{
		namespace elements {

			std::map<string, ref_ptr<IElementFactory> > Document::elementFactories = std::map<string, ref_ptr<IElementFactory> >();

			static const string s_DocShaderDefaultVertex =
				string("#version 120\n")
				+ string("attribute vec2 v_Vertex;\n")
				+ string("attribute vec4 v_Color;\n")
				+ string("attribute vec2 v_Uv1;\n")
				+ string("uniform mat4 ModelViewProjectionMatrix;\n")
				+ string("varying vec4 color;\n")
				+ string("varying vec2 uv;\n")
				+ string("void main(void) {\n")
				+ string("gl_Position = ModelViewProjectionMatrix * vec4(v_Vertex,0,1);\n")
				+ string("color = v_Color;\n")
				+ string("uv = v_Uv1;\n")
				+ string("}");
			static const string s_DocShaderDefaultFragment =
				string("#version 120\n")
				+ string("varying vec4 color;\n")
				+ string("varying vec2 uv;\n")
				+ string("uniform sampler2D Texture;\n")
				+ string("void main(void) {\n")
				+ string("vec4 texColor  = texture2D(Texture, uv);\n")
				+ string("gl_FragColor = texColor * color;\n")
				+ string("}");

			Document::Document(const ref_ptr<graphics::Context>& context, const string& basePath) :
				Element(nullptr),
				basePath(basePath),
				context(context),
				shader(new GuiShader(context, s_DocShaderDefaultVertex, s_DocShaderDefaultFragment)),
				spriteBatch(new SpriteBatch(context, shader)),
				manager(new WidgetManager(spriteBatch)),
				spriteCollection(new SpriteCollection(context, basePath)),
				focusElement(nullptr),
				boundEventHandler(std::bind(&Document::eventHandler, this, std::placeholders::_1)),
				dirtyLayout(true)
			{
				manager->needRedrawIndices = true;
				context->addEventHandler(boundEventHandler);
			}

			ref_ptr<Element> Document::createElementI(const string& elementType, const ref_ptr<Element>& parent) {
				auto factory = elementFactories.find(elementType);
				if (factory != elementFactories.end())
					return factory->second->createElement(elementType, parent);
				return new Element(parent);
			}

			void Document::setScreenSize(const vector2i& screenSize)
			{
				Document::screenSize = screenSize;
				spriteBatch->setScreenSize(screenSize.x, screenSize.y);
				updateLayout(vector4f(0, 0, screenSize.x, screenSize.y));
			}

			void Document::render()
			{
				if (dirtyLayout) {
					updateLayout(vector4f(0, 0, screenSize.x, screenSize.y));
					dirtyLayout = false;
				}

				manager->render();
				context->setDepthTestEnabled(false);
				context->setBlendingEnabled(true);
				context->setBlendFunc(graphics::BlendFunc::SRC_ALPHA, graphics::BlendFunc::ONE_MINUS_SRC_ALPHA);
				spriteBatch->draw();
			}

			void Document::setFocusElement(const ref_ptr<Element>& element)
			{
				if (focusElement != element) {
					ref_ptr<Element> tmp = focusElement;
					focusElement = element;

					if (tmp.valid())
					{
						GuiEvent guiEvent;
						guiEvent.type = GuiEventType::BLUR;
						guiEvent.target = tmp;

						dispatchEvent(guiEvent);
					}
					if (focusElement.valid())
					{
						GuiEvent guiEvent;
						guiEvent.type = GuiEventType::FOCUS;
						guiEvent.target = focusElement;
						dispatchEvent(guiEvent);
					}
				}
			}

			bool Document::eventHandler(const platform::Event& event)
			{
				if (event.type == platform::EventType::INPUT_MOUSEBUTTONUP || event.type == platform::EventType::INPUT_MOUSEBUTTONDOWN)
				{
					GuiEvent guiEvent;
					getElementAt(event.mouseButton.x, event.mouseButton.y, guiEvent.target);
					guiEvent.type = event.type == platform::EventType::INPUT_MOUSEBUTTONUP ? GuiEventType::MOUSEBUTTONUP : GuiEventType::MOUSEBUTTONDOWN;
					guiEvent.mouseButton.x = event.mouseButton.x;
					guiEvent.mouseButton.y = event.mouseButton.y;
					guiEvent.mouseButton.button = event.mouseButton.button;
					guiEvent.mouseButton.keyModifier = event.mouseButton.keyModifier;
					dispatchEvent(guiEvent);
				}
				if (event.type == platform::EventType::INPUT_MOUSEMOTION)
				{
					GuiEvent guiEvent;
					getElementAt(event.mouseMotion.x, event.mouseMotion.y, guiEvent.target);
					guiEvent.type = GuiEventType::MOUSEMOTION;
					guiEvent.mouseMotion.x = event.mouseMotion.x;
					guiEvent.mouseMotion.y = event.mouseMotion.y;
					dispatchEvent(guiEvent);
				}
				if (event.type == platform::EventType::INPUT_KEYDOWN || event.type == platform::EventType::INPUT_KEYUP)
				{
					GuiEvent guiEvent;
					guiEvent.type = event.type == platform::EventType::INPUT_KEYDOWN ? GuiEventType::KEYDOWN : GuiEventType::KEYUP;
					guiEvent.target = focusElement;
					if (!focusElement.valid())
						guiEvent.target = this;
					guiEvent.keyboard.keyModifier = event.keyboard.keyModifier;
					guiEvent.keyboard.keycode = event.keyboard.keycode;
					dispatchEvent(guiEvent);
				}
				return true;
			}

			void Document::dispatchEvent(const GuiEvent& event)
			{
				event.current = this;
				event.capture = true;
				processEvent(event);
			}

			const ref_ptr<WidgetManager>& Document::widgetManager()
			{
				return manager;
			}

			Document* Document::getDocument()
			{
				return this;
			}

			Document::~Document()
			{
				context->removeEventHandler(boundEventHandler);
			}

			void Document::registerElement(const string& tag, const ref_ptr<elements::IElementFactory>& factory) {
				elementFactories[tag] = factory;
			}

			ref_ptr<Document> Document::loadDocument(const ref_ptr<graphics::Context>& context, const string& fileName)
			{
				std::ifstream ifs(fileName);
				std::string str = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

				xml::document doc;
				doc.parse<0>(const_cast<char*>(str.data()));

				return loadDocumentFromXml(context, fileName, doc);
			}

			ref_ptr<Document> Document::loadDocumentFromXml(const ref_ptr<graphics::Context>& context, const string& fileName, const xml::node& node) {
				auto rootNode = node.first_node();
				if (rootNode == nullptr)
					throw std::exception("document is empty");

				ref_ptr<Document> doc = new Document(context, "");
				auto tagNamePtr = rootNode->name();
				string tagName = string(tagNamePtr, tagNamePtr + rootNode->name_size());
				if (tagName != "document")
					throw std::exception("expected document node!");
				doc->parseFromXml(*rootNode);
				doc->needRedraw = true;
				return doc;
			}

			void Document::registerFont(const string& name, const ref_ptr<BitmapFont>& font) {
				fonts[name] = font;
			}

			string normalizePath(string&& str) {
				return str;	
			}

			const ref_ptr<HeadElement>& Document::getHead() {
				if (head.valid())
					return head;

				for (auto child = children.cbegin(); child != children.cend(); ++child) {
					Element* el = (*child).get();
					head = dynamic_cast<HeadElement*>(el);
					if (head!= nullptr)
						return head;
				}
				head = createElement<HeadElement>();
				return head;
			}
			const ref_ptr<BodyElement>& Document::getBody() {
				if (body.valid())
					return body;

				for (auto child = children.cbegin(); child != children.cend(); ++child) {
					Element* el = (*child).get();
					body = dynamic_cast<BodyElement*>(el);
					if (body != nullptr)
						return body;
				}
				body = createElement<BodyElement>();
				return body;
			}

			ref_ptr<BitmapFont> Document::loadFont(const string& href) {
				/*auto i = fonts.find(href);
				if (i == fonts.end()) {
					ref_ptr<BitmapFont> font = BitmapFont::loadBitmapFont(spriteCollection, normalizePath(basePath + href));
					if (font.valid()) {
						string name = font->getName();
						registerFont(name, font);
					}
					return fonts[href];
				}
				return i->second;*/			

				ref_ptr<BitmapFont> rp = spriteCollection->loadBitmapFont(href);
				if (rp.valid())
					fonts[rp->getName()] = rp;
				return rp;
			}
			ref_ptr<SpriteSheet> Document::loadSpriteSheet(const string& href) {
				return spriteCollection->loadSpriteSheet(href);
			}

			ref_ptr<BitmapFont> Document::resolveFont(const string& name) {
				auto i = fonts.find(name);
				if (i != fonts.end())
					return i->second;
				return nullptr;
			}
			bool Document::resolveSprite(const string& name, SlicedSprite& sprite, ref_ptr<graphics::Texture2D>& texture) {
				return spriteCollection->tryGetSprite(name, sprite, texture);
			}
			const string& Document::getBasePath() const {
				return basePath;
			}
		}
	}
}