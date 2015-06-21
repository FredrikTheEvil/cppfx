#include "cppfx/platform/Window.h"
#include "cppfx/weak_ptr.h"
#include <cppfx/Exceptions.h>

#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_GLEW)
#include <GL/glew.h>
#endif

#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
#include <SDL.h>
#elif defined(CPPFX_USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#include <stdexcept>
#include <iostream>

using namespace cppfx::graphics;

namespace cppfx {
	namespace platform {
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
		struct CPPFX_API WindowHandle {
			SDL_Window* window;
			SDL_GLContext context;
		};
#elif defined(CPPFX_USE_GLFW)
		struct CPPFX_API WindowHandle {
			GLFWwindow* window;
			int mods = 0;
		};

		struct CPPFX_API WindowAccess
		{
			static input::KeyModifier toKeyModifier(int mods)
			{
				input::KeyModifier keyModifier = input::KeyModifier::NONE;
				if ((mods & GLFW_MOD_SHIFT) != 0)
					keyModifier = keyModifier | input::KeyModifier::SHIFT;
				if ((mods & GLFW_MOD_ALT) != 0)
					keyModifier = keyModifier | input::KeyModifier::ALT;
				if ((mods & GLFW_MOD_CONTROL) != 0)
					keyModifier = keyModifier | input::KeyModifier::CTRL;
				if ((mods & GLFW_MOD_SUPER) != 0)
					keyModifier = keyModifier | input::KeyModifier::APPLICATION;
				return keyModifier;
			}

			static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
			{
			}

			static void charmods_callback(GLFWwindow* win, unsigned int codepoint, int mods)
			{
				for (auto window = Window::windows.begin(); window != Window::windows.end(); ++window)
				{
					if ((*window)->handle->window == win)
					{
						Event e;
						e.type = EventType::INPUT_TEXT;
						e.text.text[0] = codepoint;
						e.text.text[1] = '\0';
						e.text.keyModifier = toKeyModifier(mods);
						(*window)->pushEvent(e);
						return;
					}
				}
			}

			static void cursor_position_callback(GLFWwindow* win, double xpos, double ypos)
			{
				for (auto window = Window::windows.begin(); window != Window::windows.end(); ++window)
				{
					if ((*window)->handle->window == win)
					{
						Event e;
						e.type = EventType::INPUT_MOUSEMOTION;
						e.mouseMotion.x = xpos;
						e.mouseMotion.y = ypos;
						(*window)->pushEvent(e);
						return;
					}
				}
			}

			static void processResize(GLFWwindow* win, int width, int height)
			{
				for (auto window = Window::windows.begin(); window != Window::windows.end(); ++window)
				{
					if ((*window)->handle->window == win)
					{
						Event e;
						e.type = EventType::WINDOW_RESIZE;
						e.resize.width = width;
						e.resize.height = height;
						(*window)->pushEvent(e);
						return;
					}
				}
			}
			static void mouseButton(GLFWwindow* win, int button, int pressOrRelease, int mods)
			{
				for (auto window = Window::windows.begin(); window != Window::windows.end(); ++window)
				{
					if ((*window)->handle->window == win)
					{
						Event e;
						e.type = pressOrRelease == GLFW_PRESS ? EventType::INPUT_MOUSEBUTTONDOWN : EventType::INPUT_MOUSEBUTTONUP;
						e.mouseButton.button = input::MouseButton(button);
						e.mouseButton.keyModifier = toKeyModifier(mods);
						double cx, cy;
						glfwGetCursorPos(win, &cx, &cy);
						e.mouseButton.x = int(cx);
						e.mouseButton.y = int(cy);

						(*window)->pushEvent(e);
					}
				}
			}
		};
#endif
		void Window::enumerateCapabilities()
		{
			vendorName = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
			rendererName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
			string glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
			string shadingLanguageVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
			string extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
			capsGlVersionMajor = 2;
			capsGlVersionMinor = 1;
			capsGlVersionPatch = 0;
			capsGlVersionRevision = 0;
			capsGlslVersionMajor = 1;
			capsGlslVersionMinor = 20;

			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &capsNumTexUnits);
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &capsMaxTextureSize);
			glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &capsMaxTextureSizeCube);
			textureUnits = std::vector< ref_ptr<Texture> >(capsNumTexUnits);
		}

		static bool s_windowSystemInitialized = false;

		static void cleanupWindowSystem() {
			if (!s_windowSystemInitialized)
				return;
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
#elif defined(CPPFX_USE_GLFW)
#endif
		}
		static void glfwErrorHandler(int x, const char* str)
		{
			std::cout << str << std::endl;
		}
		static void initWindowSystem() {
			if (s_windowSystemInitialized)
				return;
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
			SDL_Init(SDL_INIT_VIDEO);
#elif defined(CPPFX_USE_GLFW)
			GLFWerrorfun fn;
			glfwSetErrorCallback(glfwErrorHandler);
			if (glfwInit() == GL_FALSE)
				throw RuntimeError("failed to initialize windowing system");
#endif
			s_windowSystemInitialized = true;
			atexit(cleanupWindowSystem);
		}

		std::vector<Window*> Window::windows = std::vector<Window*>();

		Window::Window(int width, int height, bool fullscreen, const string& title)
		{
			initWindowSystem();
			handle = new WindowHandle();
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
			if(fullscreen)
				flags |= SDL_WINDOW_FULLSCREEN;

			handle->window = SDL_CreateWindow(get_string_buffer(title), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			if (handle->window == nullptr) {
				throw RuntimeError("failed to create window");
			}
			handle->context = SDL_GL_CreateContext(handle->window);
			SDL_GL_MakeCurrent(handle->window, handle->context);
#elif defined(CPPFX_USE_GLFW)
			GLFWmonitor* monitor = nullptr;
			handle->window = glfwCreateWindow(width, height, get_string_buffer(title), monitor, nullptr);
			if (handle->window == nullptr)
				throw RuntimeError("failed to create window");
			glfwMakeContextCurrent(handle->window);
			glfwSetCharModsCallback(handle->window, WindowAccess::charmods_callback);
			glfwSetKeyCallback(handle->window, WindowAccess::key_callback);
			glfwSetCursorPosCallback(handle->window, WindowAccess::cursor_position_callback);
			glfwSetMouseButtonCallback(handle->window, WindowAccess::mouseButton);
			glfwSetFramebufferSizeCallback(handle->window, WindowAccess::processResize);
#else
			CPPFX_STATIC_ASSERT(false);
#endif
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_GLEW)
			auto err = glewInit();
			if (err != GLEW_OK) {
				auto errStr = std::string(reinterpret_cast<const char*>(glewGetErrorString(err)));
				throw RuntimeError(errStr);
			}
#else
			CPPFX_STATIC_ASSERT(false);
#endif
			// Ensure defaults
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glScissor(scissorRegion.x, scissorRegion.y, scissorRegion.z, scissorRegion.w);
			glBlendFuncSeparate(GLenum(blendFuncSourceRGB), GLenum(blendFuncDestRGB), GLenum(blendFuncSourceAlpha), GLenum(blendFuncDestAlpha));
			glDepthFunc(GLenum(depthFunc));
			if (depthTestEnabled)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
			SDL_GL_SwapWindow(handle->window);
#elif defined(CPPFX_USE_GLFW)
			glfwSwapBuffers(handle->window);
#endif
			enumerateCapabilities();
			onResize(width, height);
			windows.push_back(this);
		}
		Window::~Window() {
			for (auto i = windows.begin(); i != windows.end(); ++i)
			{
				if (*i == this)
				{
					windows.erase(i);
					return;
				}
			}
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
			SDL_GL_DeleteContext(handle->context);
			SDL_DestroyWindow(handle->window);
#elif defined(CPPFX_USE_GLFW)
			glfwDestroyWindow(handle->window);
#else
			CPPFX_STATIC_ASSERT(false); // Require either SDL2 or GLFW for Window Management
#endif
			delete handle;
		}

		void Window::pushEvent(const Event& event)
		{
			eventQueue.push(event);
		}

		void Window::makeCurrent() {
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
			SDL_GL_MakeCurrent(handle->window, handle->context);
#elif defined(CPPFX_USE_GLFW)
			glfwMakeContextCurrent(handle->window);
#endif
			s_current = this;
		}

		void Window::swapBuffers()
		{
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
			SDL_GL_SwapWindow(handle->window);
#elif defined(CPPFX_USE_GLFW)
			glfwSwapBuffers(handle->window);
#endif
		}

		void Window::addClickHandler(ClickHandler handler)
		{
			clickHandlers.push_back(handler);
		}
		void Window::removeClickHandler(const ClickHandler& handler)
		{
			for (auto itr = clickHandlers.begin(); itr != clickHandlers.end(); ++itr)
			{
				if ((&(*itr)) == &handler)
				{
					clickHandlers.erase(itr);
				}
			}
		}

		bool Window::processEvents() {
#if defined(__EMSCRIPTEN__) || defined(CPPFX_USE_SDL)
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if(event.type == SDL_QUIT) {
					Event e;
					e.type = EventType::QUIT;
					eventQueue.push(e);
				}
				else if (event.type == SDL_MOUSEMOTION) {
					Event e;
					e.type = EventType::INPUT_MOUSEMOTION;
					e.mouseMotion.x = event.motion.x;
					e.mouseMotion.y = event.motion.y;
					eventQueue.push(e);
				}
				else if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
					Event e;
					e.type = event.type == SDL_MOUSEBUTTONDOWN ? EventType::INPUT_MOUSEBUTTONDOWN : EventType::INPUT_MOUSEBUTTONUP;
					e.mouseButton.button = input::MouseButton(event.button.button);
					eventQueue.push(e);
				}
				else if (event.type == SDL_WINDOWEVENT) {
					if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
						Event e;
						e.resize.width = event.window.data1;
						e.resize.height = event.window.data2;
						eventQueue.push(e);
					}
				}
			}
#elif defined(CPPFX_USE_GLFW)
			glfwPollEvents();
			if (glfwWindowShouldClose(handle->window)) {
				Event e;
				e.type = EventType::QUIT;
				eventQueue.push(e);
			}
#endif
			while (eventQueue.size() > 0)
			{
				const Event& event = eventQueue.front(); 
				if (event.type == EventType::WINDOW_RESIZE){
					onResize(event.resize.width, event.resize.height);
				}
					
				for (auto handler = eventHandlers.begin(); handler != eventHandlers.end(); ++handler)
				{
					if (!(*handler)(event))
						break;
				}

				if (event.type == EventType::QUIT) {
					eventQueue.pop();
					return false;
				}
				eventQueue.pop();
			}
			return true;
		}
	} // namespace cppfx::graphics
} // namespace cppfx