#pragma once
#ifndef __CPPFX_PLATFORM_EVENT_H
#define __CPPFX_PLATFORM_EVENT_H

#include <cppfx/config.h>
#include <cppfx/export.h>
#include <cppfx/input/Enums.h>
#include <functional>

namespace cppfx
{
	namespace platform
	{
		enum class EventType
		{
			QUIT,
			WINDOW_RESIZE,
			INPUT_KEYDOWN,
			INPUT_KEYUP,
			INPUT_TEXT,
			INPUT_MOUSEMOTION,
			INPUT_MOUSEBUTTONDOWN,
			INPUT_MOUSEBUTTONUP
		};
		struct CPPFX_API ResizeEvent
		{
			EventType type;
			int width;
			int height;
		};
		struct CPPFX_API KeyboardEvent
		{
			EventType type;
			input::KeyModifier keyModifier;
			input::Keycode keycode;
			
		};
		struct CPPFX_API MouseButtonEvent
		{
			EventType type;
			input::KeyModifier keyModifier;
			int x,  y;
			input::MouseButton button;
		};
		struct CPPFX_API MouseMotionEvent
		{
			EventType type;
			int x;
			int y;
		};
		struct CPPFX_API TextEvent
		{
			EventType type;
			input::KeyModifier keyModifier;
			char text[32];
			size_t size;
		};
		class CPPFX_API Window;
		struct CPPFX_API Event
		{
			union
			{
				struct
				{
					EventType type;
				};
				ResizeEvent resize;
				KeyboardEvent keyboard;
				MouseButtonEvent mouseButton;
				MouseMotionEvent mouseMotion;
				TextEvent text;
			};
		};
		typedef std::function<bool(const Event&)> EventHandler;
	}
}

#endif