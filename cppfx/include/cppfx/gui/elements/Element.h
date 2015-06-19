#pragma once
#ifndef __CPPFX_GUI_ELEMENTS_ELEMENT_H
#define __CPPFX_GUI_ELEMENTS_ELEMENT_H
#include <cppfx/Object.h>
#include <cppfx/vector.h>
#include <cppfx/string.h>
#include <cppfx/ref_ptr.h>
#include <vector>
#include <cppfx/gui/Widget.h>
#include <cppfx/gui/Enums.h>
#include <cppfx/gui/BitmapFont.h>
#include <cppfx/weak_ptr.h>
#include <cppfx/Xml.h>
#include <map>

namespace cppfx
{
	namespace gui
	{
		struct CPPFX_API GuiEvent;
		enum class ElementOrigin
		{
			TOP_LEFT = 0,
			TOP = 1,
			TOP_RIGHT = 2,
			LEFT = 3,
			CENTER = 4,
			RIGHT = 5,
			BOTTOM_LEFT = 6,
			BOTTOM = 7,
			BOTTOM_RIGHT = 8
		};
		typedef std::function<void(const GuiEvent&)> GuiEventHandler;
		namespace elements {
			class CPPFX_API Document;
			class CPPFX_API Element : public Object
			{
			protected:
				explicit Element(const ref_ptr<Element>& parent);
			public:
				static void registerCoreElements();

				template<typename TWidget>
				inline ref_ptr<TWidget> createWidget()
				{
					ref_ptr<TWidget> widget = new TWidget(widgetManager());
					addWidget(widget);
					return widget;
				}
				ref_ptr<Widget> createWidget(const string& widgetType);

				template<typename TElement>
				inline ref_ptr<TElement> createElement()
				{
					return new TElement(this);

				}
				ref_ptr<Element> createElement(const string& elementType);
				ref_ptr<Element> createElementFromXml(const xml::node& node);
				ref_ptr<Element> findElementById(const string& id);

				void setId(const string& newId);
				const string& getId() const;

				void setRect(const vector4f& rect);
				const vector4f& getRect() const;

				void setOrigin(ElementOrigin origin);
				ElementOrigin getOrigin() const;

				void setAlign(ElementOrigin align);
				ElementOrigin getAlign() const;

				void setAbsoluteOffset(bool abs);
				bool getAbsoluteOffset() const;
				void setAbsoluteSize(bool abs);
				bool getAbsoluteSize() const;

				void enable();
				void disable();
				bool isEnabled() const;

				void setInnerXmlString(const string& xml);
				void setInnerXml(const xml::node& node);

				virtual void setAttribute(const string& key, const string& value);
				virtual void removeAttribute(const string& key);
				virtual bool hasAttribute(const string& key) const;
				virtual string getAttribute(const string& key) const;

				virtual void focus();

				void addEventListener(GuiEventType type, GuiEventHandler handler, bool useCapture = false);
				void removeEventListener(GuiEventType type, const GuiEventHandler& handler, bool useCapture = false);
				
				bool isChild(const ref_ptr<Element>& el, bool recursive = true);

				void getElementAt(int x, int y, ref_ptr<Element>& element);
				void removeChild(const ref_ptr<Element>& element);

				virtual void update(double deltaTime);
				virtual void dispatchEvent(const GuiEvent& event);
			protected:
				void parseFromXml(const xml::node& node);
				virtual void reset();

				void addWidget(const ref_ptr<Widget>& widget);
				virtual void updateLayout(const vector4f& rect);
				virtual const ref_ptr<WidgetManager>& widgetManager();

				virtual ref_ptr<BitmapFont> resolveFont(const string& name);
				virtual bool resolveSprite(const string& name, SlicedSprite& sprite, ref_ptr<graphics::Texture2D>& texture);

				virtual Document* getDocument();
				void handleEvent(const GuiEvent& event);
				void processEvent(const GuiEvent& event);

				std::vector< ref_ptr<Element> > children;
			private:
				void clear();
				void getNextElement(ref_ptr<Element>& element, const ref_ptr<Element>& current, const ref_ptr<Element>& target);
				weak_ptr<Element> parent;
				vector4f rect;
				vector4i area;
				ElementOrigin origin;
				ElementOrigin align;
				bool absoluteOffset;
				bool absoluteSize;
				string id;
				bool disabled;
				std::vector< ref_ptr<Widget> > widgets;
				typedef std::vector<GuiEventHandler> EventHandlers;
				typedef std::map<GuiEventType, EventHandlers> EventHandlerMap;
				typedef EventHandlers::const_iterator EventHandlerIterator;
				EventHandlerMap captureEventHandlers;
				EventHandlerMap bubbleEventHandlers;
				std::unordered_map<string, string> attributes;

				Element(const Element&) = delete;
				Element& operator = (const Element) = delete;

				friend class Document;
			};
		}
	}
}

#endif