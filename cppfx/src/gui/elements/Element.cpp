#include <cppfx/gui/elements/Document.h>
#include <cppfx/gui/elements/BodyElementFactory.h>
#include <cppfx/gui/elements/HeadElementFactory.h>
#include <cppfx/gui/elements/LinkElementFactory.h>
#include <cppfx/gui/elements/SpriteElementFactory.h>
#include <cppfx/gui/elements/LabelElementFactory.h>
#include <cppfx/gui/GuiEvent.h>
#include <cppfx/gui/Widget.h>

namespace cppfx
{
	namespace gui
	{
		namespace elements {
			static bool s_coreElementsRegistered = false;
			void Element::registerCoreElements() {
				if (!s_coreElementsRegistered) {
					s_coreElementsRegistered = true;

					Document::registerElement("body", new BodyElementFactory());
					Document::registerElement("head", new HeadElementFactory());
					Document::registerElement("link", new LinkElementFactory());
					Document::registerElement("sprite", new SpriteElementFactory());
					Document::registerElement("label", new LabelElementFactory());
				}
			}

			ref_ptr<Widget> Element::createWidget(const string& widgetType)
			{
				//return widgetManager()->createWidget(widgetType);
				return nullptr;
			}

			ref_ptr<Element> Element::createElement(const string& elementType)
			{
				auto doc = getDocument();
				if (doc != nullptr)
					return doc->createElementI(elementType, this);
				return nullptr;
			}

			void Element::setInnerXmlString(const string& xml) {
			}
			void Element::setInnerXml(const xml::node& node) {
				clear();
				std::string tagName;
				for (auto itr = node.first_node(); itr != nullptr; itr = itr->next_sibling())
					createElementFromXml(*itr);
			}

			void Element::setRect(const vector4f& rect)
			{
				if (Element::rect != rect) {
					Element::rect = rect;
				}
			}

			const vector4f& Element::getRect() const
			{
				return rect;
			}

			void Element::setOrigin(ElementOrigin origin)
			{
				Element::origin = origin;
			}

			ElementOrigin Element::getOrigin() const
			{
				return origin;
			}

			void Element::setAlign(ElementOrigin align)
			{
				Element::align = align;
			}
			ElementOrigin Element::getAlign() const {
				return align;
			}

			void Element::setAbsoluteOffset(bool abs)
			{
				absoluteOffset = abs;
			}

			bool Element::getAbsoluteOffset() const
			{
				return absoluteOffset;
			}

			void Element::setAbsoluteSize(bool abs)
			{
				absoluteSize = abs;
			}

			bool Element::getAbsoluteSize() const
			{
				return absoluteSize;
			}

			void Element::focus()
			{
				ref_ptr<Document> doc = getDocument();
				if (doc.valid())
					doc->setFocusElement(this);
			}

			Element::Element(const ref_ptr<Element>& parent) :
				parent(parent.get()),
				origin(ElementOrigin::TOP_LEFT),
				align(ElementOrigin::TOP_LEFT),
				absoluteOffset(false),
				absoluteSize(false),
				id(),
				disabled(false),
				rect(0.0f, 0.0f, 1.0f, 1.0f)
			{
				if (parent.valid()) {
					parent->children.push_back(this);
					GuiEvent e;
					e.type = GuiEventType::DOMNODEADDED;
					e.target = this;
					parent->dispatchEvent(e);
				}
			}

			void Element::addWidget(const ref_ptr<Widget>& widget)
			{
				widgets.push_back(widget);
			}

			void Element::update(double time)
			{
				for (auto w = widgets.begin(); w != widgets.end(); ++w)
					(*w)->update(time);

				for (auto child = children.begin(); child != children.end(); ++child)
					(*child)->update(time);
			}

			void Element::addEventListener(GuiEventType type, GuiEventHandler handler, bool useCapture)
			{
				if (useCapture) {
					auto i = captureEventHandlers.find(type);
					if (i != captureEventHandlers.end())
					{
						i->second.push_back(handler);
					}
					else
					{
						EventHandlers handlers = { handler };
						captureEventHandlers.insert(std::pair<GuiEventType, EventHandlers>(type, handlers));
					}
				}
				else {
					auto i = bubbleEventHandlers.find(type);
					if (i != bubbleEventHandlers.end())
					{
						i->second.push_back(handler);
					}
					else
					{
						EventHandlers handlers = { handler };
						bubbleEventHandlers.insert(std::pair<GuiEventType, EventHandlers>(type, handlers));
					}
				}
			}

			void Element::removeEventListener(GuiEventType type, const GuiEventHandler& handler, bool useCapture)
			{
				EventHandlerMap& map = useCapture ? captureEventHandlers : bubbleEventHandlers;
				auto i = map.find(type);
				if (i != map.end())
					map.erase(i);
			}

			void Element::getElementAt(int x, int y, ref_ptr<Element>& element)
			{
				element = this;
				for (auto child_itr = children.begin(); child_itr != children.end(); ++child_itr)
				{
					Element& el = *(*child_itr);
					if (x >= el.area.x && y >= el.area.y &&
						x < el.area.x + el.area.z &&
						y < el.area.y + el.area.w)
					{
						el.getElementAt(x, y, element);
						return;
					}
				}
			}

			ref_ptr<Element> Element::findElementById(const string& id) {
				if (Element::id == id)
					return this;

				Document* doc = getDocument();
				if (doc != nullptr) {
					ref_ptr<Element> el;
					if (doc->idCache.tryGet(id, el)) {
						if (isChild(el))
							return el;
					}
				}
				return nullptr;
			}

			void Element::removeChild(const ref_ptr<Element>& element)
			{
				for (auto itr = children.begin(); itr != children.end(); ++itr)
				{
					if (*itr == element)
					{
						(*itr)->setId("");

						GuiEvent event;
						event.type = GuiEventType::DOMNODEREMOVED;
						event.target = element;
						dispatchEvent(event);
						children.erase(itr);
						return;
					}
				}
			}

			ref_ptr<BitmapFont> Element::resolveFont(const string& name) {
				if (parent.valid())
					return parent->resolveFont(name);
				return nullptr;
			}
			bool Element::resolveSprite(const string& name, SlicedSprite& sprite, ref_ptr<graphics::Texture2D>& texture) {
				if (parent.valid())
					return parent->resolveSprite(name, sprite, texture);
				return false;
			}

			void Element::dispatchEvent(const GuiEvent& event)
			{
				if (parent.valid())
					parent->dispatchEvent(event);
			}

			inline bool tryGetAttribInt(const xml::node& node, const char* name, int& x)
			{
				auto attrib = node.first_attribute(name);
				if (attrib != nullptr)
				{
					string x = string(static_cast<char*>(attrib->value()), static_cast<char*>(attrib->value()) + attrib->value_size());
					x = std::stoi(x);
					return true;
				}
				return false;
			}
			inline bool tryGetAttribFloat(const xml::node& node, const char* name, float& x)
			{
				auto attrib = node.first_attribute(name);
				if (attrib != nullptr)
				{
					string x = string(static_cast<char*>(attrib->value()), static_cast<char*>(attrib->value()) + attrib->value_size());
					x = std::stof(x);
					return true;
				}
				return false;
			}

			inline bool tryGetAttribString(const xml::node& node, const char* name, string& x)
			{
				auto attrib = node.first_attribute(name);
				if (attrib != nullptr)
				{
					x = string(static_cast<char*>(attrib->value()), static_cast<char*>(attrib->value()) + attrib->value_size());
					return true;
				}
				return false;
			}

			bool Element::isChild(const ref_ptr<Element>& el, bool recursive)
			{
				if (recursive) {
					for (auto parent = el->parent; parent != nullptr; parent = parent->parent)
						if (parent == this)
							return true;
					return false;
				}
				return el->parent == this;
			}

			void Element::setId(const string& newId) {
				if (id != newId) {
					Document* doc = getDocument();
					if (doc != nullptr) {
						if (newId.size() > 0) {
							if (doc->idCache.contains(newId))
								throw std::exception("");
							doc->idCache.insert(newId, this);
						}
						if (id.size() > 0)
							doc->idCache.erase(id);
						id = newId;
					}
				}
			}
			const string& Element::getId() const {
				return id;
			}

			bool Element::hasAttribute(const string& key) const {
				if (key == "id")
					return id.size() > 0;
				else if (key == "left")
					return rect.x != 0.0f;
				else if (key == "right")
					return rect.z != 1.0f;
				else if (key == "top")
					return rect.y != 0.0f;
				else if (key == "bottom")
					return rect.w != 1.0f;
				else if (key == "disabled")
					return disabled;
				else if (key == "origin")
					return origin != ElementOrigin::TOP_LEFT;
				else if (key == "align")
					return align != ElementOrigin::TOP_LEFT;
				else if (key == "absolute-size")
					return absoluteSize;
				else if (key == "absolute-offset")
					return absoluteOffset;
				else
					return attributes.find(key) != attributes.end();
			}

			void Element::enable()
			{
			}
			void Element::disable()
			{
			}

			void Element::setAttribute(const string& key, const string& value)
			{
				if (key == "id") {
					setId(value);
				}
				else if (key == "x")
				{
					setRect(vector4f(std::stof(value), rect.y, rect.z, rect.w));
				}
				else if (key == "width")
				{
					setRect(vector4f(rect.x, rect.y, std::stof(value), rect.w));
				}
				else if (key == "y")
				{
					setRect(vector4f(rect.x, std::stof(value), rect.z, rect.w));
				}
				else if (key == "height")
				{
					setRect(vector4f(rect.x, rect.y, rect.z, std::stof(value)));
				}
				else if (key == "origin")
				{
					if (value == "top-left")
						setOrigin(ElementOrigin::TOP_LEFT);
					else if (value == "top")
						setOrigin(ElementOrigin::TOP);
					else if (value == "top-right")
						setOrigin(ElementOrigin::TOP_RIGHT);
					else if (value == "left")
						setOrigin(ElementOrigin::LEFT);
					else if (value == "center")
						setOrigin(ElementOrigin::CENTER);
					else if (value == "right")
						setOrigin(ElementOrigin::RIGHT);
					else if (value == "bottom-left")
						setOrigin(ElementOrigin::BOTTOM_LEFT);
					else if (value == "bottom")
						setOrigin(ElementOrigin::BOTTOM);
					else if (value == "bottom-right")
						setOrigin(ElementOrigin::BOTTOM_RIGHT);
				}
				else if (key == "align")
				{
					if (value == "top-left")
						setAlign(ElementOrigin::TOP_LEFT);
					else if (value == "top")
						setAlign(ElementOrigin::TOP);
					else if (value == "top-right")
						setAlign(ElementOrigin::TOP_RIGHT);
					else if (value == "left")
						setAlign(ElementOrigin::LEFT);
					else if (value == "center")
						setAlign(ElementOrigin::CENTER);
					else if (value == "right")
						setAlign(ElementOrigin::RIGHT);
					else if (value == "bottom-left")
						setAlign(ElementOrigin::BOTTOM_LEFT);
					else if (value == "bottom")
						setAlign(ElementOrigin::BOTTOM);
					else if (value == "bottom-right")
						setAlign(ElementOrigin::BOTTOM_RIGHT);
				}
				else if (key == "absolute-offset")
				{
					setAbsoluteOffset(true);
				}
				else if (key == "absolute-size")
				{
					setAbsoluteSize(true);
				}
				else if (key == "disabled")
				{
					disable();
				}
			}

			static char attrib_buffer[512];

			string Element::getAttribute(const string& key) const
			{
				if (key == "id")
				{
					return id;
				}
				else if (key == "x")
				{
					return std::to_string(rect.x);
				}
				else if (key == "width")
				{
					return std::to_string(rect.z);
				}
				else if (key == "y")
				{
					return std::to_string(rect.y);
				}
				else if (key == "height")
				{
					return std::to_string(rect.w);
				}
				else if (key == "origin") {
				}
				else if (key == "align") {
				}
				else if (key == "absolute-offset")
					return absoluteOffset ? "true" : "";
				else if (key == "absolute-size")
					return absoluteSize ? "true" : "";
				else if (key == "disabled")
				{
					return disabled ? "disabled" : "";
				}
				auto i = attributes.find(key);
				if (i != attributes.end())
					return i->second;
				return "";
			}

			void Element::removeAttribute(const string& key)
			{
				if (key == "id")
					setId("");
				else if (key == "x")
					setRect(vector4f(0.0f, rect.y, rect.z, rect.w));
				else if (key == "width")
					setRect(vector4f(rect.x, rect.y, 1.0f, rect.w));
				else if (key == "y")
					setRect(vector4f(rect.x, 0.0f, rect.z, rect.w));
				else if (key == "height")
					setRect(vector4f(rect.x, rect.y, rect.z, 1.0f));
				else if (key == "origin")
					setOrigin(ElementOrigin::TOP_LEFT);
				else if (key == "align")
					setAlign(ElementOrigin::TOP_LEFT);
				else if (key == "absolute-size")
					setAbsoluteSize(false);
				else if (key == "absolute-offset")
					setAbsoluteOffset(false);
				else if (key == "disabled")
					enable();
			}

			bool Element::isEnabled() const {
				return !disabled;
			}

			ref_ptr<Element> Element::createElementFromXml(const xml::node& node)
			{
				auto tagNamePtr = node.name();
				auto tagName = string(tagNamePtr, tagNamePtr + node.name_size());

				ref_ptr<Element> el = createElement(tagName);
				el->parseFromXml(node);
				return el;
			}

			void Element::reset() {
			}


			void Element::parseFromXml(const xml::node& node)
			{
				clear();
				reset();

				string key;
				string value;

				for (auto attrib = node.first_attribute(); attrib != nullptr; attrib = attrib->next_attribute())
				{
					auto ptr = static_cast<char*>(attrib->name());
					key = string(ptr, ptr + attrib->name_size());
					ptr = static_cast<char*>(attrib->value());
					value = string(ptr, ptr + attrib->value_size());

					setAttribute(key, value);
				}

				std::string tagName;
				for (auto itr = node.first_node(); itr != nullptr; itr = itr->next_sibling())
					createElementFromXml(*itr);
			}

			void Element::updateLayout(const vector4f& area)
			{
				auto tmpArea = vector4i(int(area.x), int(area.y), int(area.z), int(area.w));
				if (Element::area != tmpArea) {
					for (auto w = widgets.begin(); w != widgets.end(); ++w)
						(*w)->setArea(tmpArea);
					Element::area = tmpArea;
				}
				for (auto child = children.begin(); child != children.end(); ++child)
				{
					const ref_ptr<Element>& element = *child;
					Element& el = *element;
					vector4f rect;

					if (el.absoluteOffset) {
						rect.x = el.rect.x;
						rect.y = el.rect.y;
					}
					else {
						rect.x = el.rect.x * area.z;
						rect.y = el.rect.y * area.w;
					}
					if (el.absoluteSize) {
						rect.z = el.rect.z;
						rect.w = el.rect.w;
					}
					else {
						rect.z = area.z * el.rect.z;
						rect.w = area.w * el.rect.w;
					}
					if (el.origin == ElementOrigin::CENTER)
					{
						rect.x -= rect.z * 0.5f;
						rect.y -= rect.w * 0.5f;
					}
					else if (el.origin == ElementOrigin::BOTTOM)
					{
						rect.x -= rect.z * 0.5f;
						rect.y -= rect.w;
					}
					else if (el.origin == ElementOrigin::RIGHT)
					{
						rect.x -= rect.z;
						rect.y -= rect.w * 0.5f;
					}
					else if (el.origin == ElementOrigin::LEFT) {
						rect.x += rect.z;
						rect.y -= rect.w * 0.5f;
					}
					else if (el.origin == ElementOrigin::BOTTOM_RIGHT)
					{
						rect.x -= rect.z;
						rect.y -= rect.w;
					}
					else if (el.origin == ElementOrigin::TOP) {
						rect.x -= rect.z * 0.5f;
					}
					else if (el.origin == ElementOrigin::TOP_RIGHT)
						rect.x -= rect.z;
					else if (el.origin == ElementOrigin::BOTTOM_LEFT)
						rect.y -= rect.w;

					if (el.align == ElementOrigin::TOP_LEFT) {
						rect.x += area.x;
						rect.y += area.y;
					}
					else if (el.align == ElementOrigin::TOP_RIGHT) {
						rect.x += area.x + area.z;
						rect.y += area.y;
					}
					else if (el.align == ElementOrigin::TOP) {
						rect.x += area.x + area.z * 0.5f;
						rect.y += area.y;
					}
					else if (el.align == ElementOrigin::LEFT) {
						rect.x += area.x;
						rect.y += area.y + area.w * 0.5f;
					}
					else if (el.align == ElementOrigin::CENTER) {
						rect.x += area.x + area.z * 0.5f;
						rect.y += area.y + area.w * 0.5f;
					}
					else if (el.align == ElementOrigin::RIGHT) {
						rect.x += area.x + area.z;
						rect.y += area.y + area.w * 0.5f;
					}
					else if (el.align == ElementOrigin::BOTTOM_LEFT) {
						rect.x += area.x;
						rect.y += area.x + area.w;
					}
					else if (el.align == ElementOrigin::BOTTOM) {
						rect.x += area.x + area.z * 0.5f;
						rect.y += area.x + area.w;
					}
					else if (el.align == ElementOrigin::BOTTOM_RIGHT) {
						rect.x += area.x + area.z;
						rect.y += area.x + area.w;
					}

					el.updateLayout(rect);
				}
			}

			const ref_ptr<WidgetManager>& Element::widgetManager()
			{
				if (parent != nullptr)
					return parent->widgetManager();
				return nullptr;
			}

			Document* Element::getDocument()
			{
				if (parent != nullptr)
					return parent->getDocument();
				return nullptr;
			}

			void Element::clear()
			{
				while (children.size() > 0)
					removeChild(children.front());
			}

			void Element::getNextElement(ref_ptr<Element>& element, const ref_ptr<Element>& current, const ref_ptr<Element>& target)
			{
				if (current == target)
				{
					element = nullptr;
					return;
				}

				element = target;
				while (element->parent != current && element != nullptr)
				{
					if (!element->parent)
					{
						element = nullptr;
						return;
					}
					element = element->parent;
				}
			}

			void Element::handleEvent(const GuiEvent& event)
			{
				bool capture = event.capture;
				ref_ptr<Element> current = event.current;
				EventHandlerIterator begin;
				EventHandlerIterator end;
				if (capture)
				{
					auto i = captureEventHandlers.find(event.type);
					if (i != captureEventHandlers.end()) {

						for (auto itr = i->second.begin(); itr != i->second.end(); ++itr)
						{
							event.capture = capture;
							event.current = current;
							(*itr)(event);
							if (event.stopPropagation)
								return;
						}
					}
				}
				else
				{
					auto i = bubbleEventHandlers.find(event.type);
					if (i != bubbleEventHandlers.end()) {
						for (auto itr = i->second.begin(); itr != i->second.end(); ++itr)
						{
							event.capture = capture;
							event.current = current;
							(*itr)(event);
							if (event.stopPropagation)
								return;
						}
					}
				}
			}
			void Element::processEvent(const GuiEvent& event)
			{
				event.current = this;
				event.capture = true;

				// Handle Capture Event
				handleEvent(event);
				if (event.stopPropagation)
					return;

				ref_ptr<Element> next;
				getNextElement(next, event.current, event.target);
				if (next.valid())
					next->processEvent(event);
				if (event.stopPropagation)
					return;

				event.current = this;
				event.capture = false;

				// Handle Bubble Event
				handleEvent(event);
			}
		}
	}
}