#pragma once
#ifndef __CPPFX_PLATFORM_WINDOW_H
#define __CPPFX_PLATFORM_WINDOW_H

#include <cppfx/string.h>
#include <cppfx/graphics/Context.h>

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(push)
#	pragma warning( disable : 4251 )
#endif
#include <functional>
#include "Event.h"
#include <queue>

namespace cppfx {
	namespace platform {
		struct CPPFX_API WindowAccess;
		struct CPPFX_API WindowHandle;
		class CPPFX_API Window : public graphics::Context {
		public:
			Window(int width, int height, bool fullscreen, const string& title);
			bool processEvents() override;
			void makeCurrent() override;
			void swapBuffers() override;

			typedef std::function<void(int, int, int)> ClickHandler;

			void addClickHandler(ClickHandler handler);
			void removeClickHandler(const ClickHandler& handler);
		protected:
			~Window();
		private:
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;

			string title;
			WindowHandle* handle ;

			void pushEvent(const Event& event);

			std::queue<Event> eventQueue;
			
			std::vector<ClickHandler> clickHandlers;
			static std::vector<Window*> windows;

			friend struct WindowAccess;
		};
	} // namespace cppfx::platform
} // namespace cppfx

#if _MSC_VER && !__INTEL_COMPILER
#	pragma warning(pop)
#endif

#endif