#pragma once
#ifndef __CPPFX_GUI_GUIEVENT_H
#define __CPPFX_GUI_GUIEVENT_H

#include <cppfx/gui/elements/Element.h>

namespace cppfx
{
	namespace gui
	{
		struct CPPFX_API GuiEventMouseButton
		{
			GuiEventType type;
			input::KeyModifier keyModifier;
			int x, y;
			input::MouseButton button;
		};
		struct CPPFX_API GuiEventMouseMotion
		{
			GuiEventType type;
			input::KeyModifier keyModifier;
			int x, y;
		};
		struct CPPFX_API GuiEventKeyboard
		{
			GuiEventType type;
			input::KeyModifier keyModifier;
			input::Keycode keycode;
		};
		struct CPPFX_API GuiEventClick
		{
			GuiEventType type;
			input::KeyModifier keyModifier;
		};
		struct CPPFX_API GuiEvent
		{
			union
			{
				GuiEventType type;
				GuiEventClick click;
				GuiEventMouseButton mouseButton;
				GuiEventMouseMotion mouseMotion;
				GuiEventKeyboard keyboard;
			};
			ref_ptr<elements::Element> target;
			mutable ref_ptr<elements::Element> current;
			mutable bool capture = false;
			mutable bool stopPropagation = false;
		};
	}
}

#endif