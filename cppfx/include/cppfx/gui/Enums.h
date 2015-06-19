#pragma once
#ifndef __CPPFX_GUI_ENUMS_H
#define __CPPFX_GUI_ENUMS_H

namespace cppfx
{
	namespace gui
	{
		enum class MouseButton : int {
			LEFT = 0,
			RIGHT = 1,
			MIDDLE = 2
		};
		enum class GuiEventType
		{
			MOUSEMOTION,
			MOUSEBUTTONDOWN,
			MOUSEBUTTONUP,
			KEYDOWN,
			KEYUP,
			TEXTINPUT,
			FOCUS,
			BLUR,
			CHANGE,
			DOMNODEADDED,
			DOMNODEREMOVED
		};
	}
}

#endif